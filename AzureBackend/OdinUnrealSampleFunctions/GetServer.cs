using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Azure.Functions.Worker;
using Microsoft.Extensions.Logging;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Net.Http.Json;
using System.Text.Json;
using System.Threading.Tasks;

namespace OdinUnrealSampleFunctions
{
    public class GetServer
    {
        private readonly ILogger<GetServer> _logger;

        private static readonly HttpClient client = new();

        public GetServer(ILogger<GetServer> logger)
        {
            _logger = logger;
        }

        [Function("GetServer")]
        public async Task<IActionResult> Run([HttpTrigger(AuthorizationLevel.Anonymous, "get")] HttpRequest req)
        {
            var appId = Environment.GetEnvironmentVariable("ODIN_APP_ID");
            var bearerToken = Environment.GetEnvironmentVariable("ODIN_API_TOKEN");
            var apiUrl = $"https://fleet.4players.io/api/v1/apps/{appId}/services";

            client.DefaultRequestHeaders.Authorization = new AuthenticationHeaderValue("Bearer", bearerToken);

            var response = await client.GetAsync(apiUrl);
            if (!response.IsSuccessStatusCode)
            {
                _logger.LogError($"Odin API call failed with status: {response.StatusCode}");
                return new StatusCodeResult((int)response.StatusCode);
            }

            var content = await response.Content.ReadAsStringAsync();
            var json = JsonDocument.Parse(content);
            var services = json.RootElement.GetProperty("data");

            foreach (var service in services.EnumerateArray())
            {
                var status = service.GetProperty("status").GetString();
                var isHealthy = service.GetProperty("isHealthy").GetBoolean();
                var node = service.GetProperty("node");
                var ip = node.GetProperty("address").GetString();

                if (status == "running" && isHealthy && !string.IsNullOrEmpty(ip))
                {
                    var ports = service.GetProperty("ports");

                    foreach (var port in ports.EnumerateArray())
                    {
                        var protocol = port.GetProperty("protocol").GetString();
                        var publishedPort = port.GetProperty("publishedPort").GetInt32();

                        if (protocol == "udp" && publishedPort > 0)
                        {
                            return new OkObjectResult(new
                            {
                                ip,
                                port = publishedPort
                            });
                        }
                    }
                }
            }

            return new NotFoundObjectResult("No running and healthy service with a UDP port was found.");
        }
    }
}
