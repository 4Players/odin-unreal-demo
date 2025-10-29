# Odin Unreal Demo

This is a simple demonstration of the usage of the [Unreal SDK](https://github.com/4Players/odin-sdk) of 4Player's ODIN, a Voice Chat full service solution. The SDK provides an audio stream that can be processed in the audio engine of Unreal to add spatialization or audio effects.

This version is built upon the [Unreal Engine's Networking Solution](https://docs.unrealengine.com/4.27/en-US/InteractiveExperiences/Networking/Overview/). This sample works for Unreal Engine 5.3+. The sample's most recent release holds a client built with UE5.5 that connects with a dedicated server at [Odin Fleet](https://odin.4players.io/fleet/).

An alternative version with Photon instead of Unreal Networking can be found on the [Photon Networking Branch](https://github.com/4Players/odin-unreal-demo/tree/photon-networking). That branch works for UE4.26+.

## Features

The Demo showcases the usage of ODIN together with Unreal's Audio Engine. The example uses two Odin voice chat rooms - one for spatialized (proximity) chat, and one for a radio simulation with audio effects applied. It shows:

- how to connect to an ODIN voice chat room with an acquired room token generated from your access key
- how to attach the incoming audio stream to an ODIN synth component, adding spatialization (3D / proximity) 
- how to increase immersion by adding audio occlusion effects through collision rays
- how to send the voice chat audio stream to audio effect processors, e.g. for environmental effects that also apply to voice
- how to use a second ODIN room for simulating 2D radio communication (press and hold R key while talking)
- how to activate different filters (APMs) to improve the audio capture quality

## Getting Started

In the releases you can find a pre-built game executable and the current project's code base.  The pre-built game can likely be executed without any installations required - if you do not have the redistributable packages of C++ installed, that Unreal needs, you can find it in the `\Engine\Extras\Redist\en-us` folder of the pre-built game. There is one client available per platform, that uses a custom Online Subsystem to connect to the dedicated [Odin Fleet Server](https://odin.4players.io/fleet/). The sample supports more Online Subsystems that can be used:
1. OdinFleet Online Subsystem: This Subsystem connects to a custom [Azure Functions REST API](https://azure.microsoft.com/en-us/products/functions) that queries Odin Fleet for the first free running server of the demo. The client then connects to that dedicated server. This version does not need any user account and works outside a local network - there is only one global session available that is accessed by all clients for demo purposes.
1. EOS Online Subsystem: This uses EOS for matchmaking. This version of the demo game needs the user to login to their Epic Games Account and allow the Demo Game to access basic information (user name and online presence).
1. No Online Subsystem (also called "NULL"): This uses matchmaking over a local network. No account needed for this version but you will only be able to find other clients in the same local network.

To open the project in the Unreal Editor you need to install the Unreal Engine 5.5 or higher. You can open the source code with the corresponding version of the branch easily. If you need to open it with a higher version of the Unreal Engine you can right-click the `OdinUnrealSample.uproject` file and `Switch Unreal Engine version ...` to the Engine version you have installed that you want the project to open with. If the Editor fails to launch, rebuild the project from Visual Studio. You might also need to exchange the Odin Plugin to the corresponding version, downloadable e.g. in its [Github Repository](https://github.com/4Players/odin-sdk-unreal/releases).

:warning: In UE5.4+ currently there is a bug, which causes UnfocusedVolumeMultiplier to not apply to the Odin Synth - so testing is best done on 2 devices instead of starting two clients in the editor process.

To change the Online Subsystem in the Unreal project open the `Config\DefaultEngine.ini` and change the lines
```
[OnlineSubsystem]
DefaultPlatformService=OdinFleet
```
to
```
[OnlineSubsystem]
DefaultPlatformService=NULL
```
or
```
[OnlineSubsystem]
DefaultPlatformService=EOS
```

### Installing Visual Studio and Compiling the Project

As stated above, sometimes it is necessary to compile from Visual Studio:

1. Download the newest version of Visual Studio: https://visualstudio.microsoft.com/de/
2. Community Edition is fine for this project.
3. Open the installer.
4. Make sure to enable "Game development with C++" Workload. And then include "Unreal Engine installer" and "Windows 10 SDK" in its options on the right side of the Installer's Window.
5. You might need to right-click the `OdinUnrealSample.uproject` file and `Generate Visual Studio project files`.
6. Double-click the `OdinUnrealSample.uproject`
7. It should compile automatically now. If this does not work, open the `OdinUnrealSample.sln` file instead.
8. Visual Studio will open.
9. Once done hit F5 to start the debugger - and compile using Visual Studio.

### Sample Map

Below you can see an overview of the map, its areas and what they show:

![MapOverview.png](/Documentation/img/MapOverview.png)

The audio sources in the level have the same attenuation settings as the positional voice chat and so you can use them to get an idea of what the level does without having to invite someone else to test with you. To turn an audio source on, go near it and then press space on your keyboard.

## Project Structure

*This guide is written for Unreal Engine version 5.5 as the project is targeted for this version of the engine. You will need to adjust for Unreal Engine 5.4 and above accordingly.*

The project is based on the **Top Down Example** by Epic Games, so most of its structure can be found in this project as well.

This document outlines the structure of this sample project and describes how you can achieve similar results in your Unreal project. First we will have a look at the overall folder structure of the project so you can find every asset. Next we will outline the Game Mode, Game Instance and Player Controller classes to create the basic game rules and input methods. After that we describe the Photon Networking of this project and how it replicates the player characters' positions to all clients before we look at how we incorporate Odin here so that each character gets the correct Odin Media assigned. Lastly we look at the map and see what features of the Unreal Audio Engine are show cased here in which way.

### Content Directory

Most of the project's logic is located inside the `Blueprints` folder. Other folders hold the needed fonts for texts, meshes, materials and so on. In the `Sound` folder you can find the used attenuation settings and sound effect assets. Depending on which audio engine you whish to use they may differ from what you need in your project. In order to get an idea of what they do and how the settings affect the audio output of the project you can check out the documentation on [Sound in General in the Unreal Engine](https://docs.unrealengine.com/5.3/en-US/WorkingWithAudio/), and [Sound Attenuation in the Unreal Engine](https://docs.unrealengine.com/5.3/en-US/WorkingWithAudio/DistanceModelAttenuation/) specifically as well as [Submixes](https://docs.unrealengine.com/5.4/en-US/WorkingWithAudio/Submixes/).

The `Maps` folder holds both the map for the Lobby UI and the default map of the project. The `Lobby` map simply overrides the standard Game Mode with a Lobby Game Mode that opens the needed UI. The main map is the `TopDownExampleMap` in here.

In the `Blueprint/Game` Folder you can find the Game Instance, Game Mode, Player Controller and Player Character Classes - these are the entry points for the engine to our game logic, so we will have a look at them in the next paragraph of this guide.

Next to the `Game` folder you can find directories for each different kind of objects in the project - `Interactive Objects` holds an Actor for the playback of a simple sound that uses the same attenuation settings as the Odin integration so that you can compare the Odin Audio to the internal sounds directly produced in the engine. In `Lobby` you can find an additional Game Mode for the Lobby Menu that simply loads the corresponding UI. In `Networking` you can find everything related to the Networking logic. `Odin` holds the custom Actor Component we have built for this project as well as some helper classes. Lastly, in the `Widgets` folder you can find any widget that is used in the project - they are built in a way that they do not need to know of the Odin SDK, so that you do not have to look into these classes. Of course it can make sense for you to copy and paste widgets from the `Options Menu` for use in your own game as the needed UI for the Odin Options most likely stays roughly the same for each application.

![ProjectFolderStructure.png](/Documentation/img/ProjectFolderStructure.png)

Now that you have an overview of the Project in general, we can have a look at the Standard Game Mode and its related classes.

### Game Mode, Game Instance and Player Controller

The Game Mode `Top Down Game Mode` used in the game's main map has almost no logic in it. It  defines the used default classes  - we have changed the default player controller to the `TopDownController` class and the default pawn class to `TopDownCharacter`.

![GameModeSettings.png](/Documentation/img/GameModeSettings.png)

Besides these we have some functions to manage a list of all available Player Characters. The Game Mode is a singleton for each game session and thus can be used to generate networking IDs for each joining player. 

In the project settings (in the main menu bar go to `Edit` and then `Project Settings...`) we also needed to change the game instance class to `GameInstance_TD` in the Maps&Modes category. 

![GameInstance.png](/Documentation/img/GameInstance.png)

You can also just change your default GameMode here or you can define it on a per-map basis which we did in this project. To do so, open the map, go to `Blueprints` above the renderer panel and select your game mode under `World Overrides -> Game Mode`.

![MapOverrideGameMode.png](/Documentation/img/MapOverrideGameMode.png)

The Game Instance has no logic as well - it just holds a string variable `Player Name` that the player may choose in the Lobby and that needs to be saved when loading the main map. Here it is used to identify a player's character on other clients - so we will just copy the player name and replicate it to the other clients. We will do so in a minute. Besides that we have a Version that is shown in the main menu and we have a `Save Game` object that serializes the user's current settings.

Next we will have a look at the `Top Down Controller`. The logic in here fully relates to inputs - they allow switching between a top down paradigm to first person perspective and back at any time to help play around with the immersion coming from the 3D Audio.

More important for the Controller are the assigned Components. The `C_UnrealNetworking` is our networking component that we will use to replicate the game state, `OdinClientComponent` is the custom Component handling all the logic for the communication with Odin and `UiManager` simply handles some of the UI shown on screen - in our case it just adds and removes widgets reflecting connected players.

![ControllerComponents.png](/Documentation/img/ControllerComponents.png)

In the next part we will now look at what the Networking Component does exactly.

### Unreal Networking Component and Replication

The `C_UnrealNetworkingComponent` class can be found in the `Blueprints/Networking` folder. It inherits from the Actor Componen. The Component extends the class to handle our use case - yet it is still agnostic to the Odin Framework and thus implements the `Networking Component` Blueprint Interface so that other Components can communicate with the implemented networking framework without knowing which one is used exactly. You can easily extend or exchange this component for anything you would like to use in your application.

The `Networking Component` heavily uses Unreal Engine's replication feature. It is designed to behave correctly on both the listen server as well as the client.

In the `BeginPlay` event or `OnPossess` event respectively (if we are on the listen server) we create a reference to the client's player character. Additionally, on the server, we let the Game Mode know of the new player and register a handler for every new player that joins the session.

Lastly `OnEndPlay` we destroy the session to disconnect the client from it - or completely destroy it, if we are on the server.

### Odin Client Component

The `Odin Client Component` is the meat and bone of this sample so we will have a detailed look at what it does.

It derives directly from `Actor Component` and does not implement any interfaces. It has two sets of functions - one for each ODIN room it connects to - remember that we have a proximity chat and a radio chat so that we need to join two ODIN rooms in total. You can either connect to them in the same function and then decide how to handle the different events with the passed `Room` parameter, or do it like in this sample and break them into two parts.

The `Start Connect` Custom Event is called once the local player is connected to the Photon room. It is mainly taken from the [ODIN Unreal SDK Manual](https://docs.4players.io/voice/unreal/manual/). In summary, we generate a room token with our access key, a room id and a user id. Note, that the Access Key should not be known to the client normally and you should do this on a trusted Web Server instead. For the simplicity of this sample we kept the call on the client though.

Then we construct a room and bind all needed events to appropriate Custom Events (we go through them in a bit). Then we create some User Data - they contain the chosen User Name from the Login Screen and the Network Id from our Photon Component. The Network Id is needed by the other clients so that they can get the correct Player Character from the Photon Component to assign it an Odin Synth Component with the correct Media Stream. Once that is done, we will call the `Join Room` function and are done with the event. We do this two times in total, once for each Odin Room.

We handle the Room Events in different events:

- `Peer Joined`: This event is called before the media of the other peer is added to the room, so we can setup the player character - we use the network id in the user data and the passed ODIN peer id to create a map on this component, so once we get the media stream with the ODIN id we can get the correct network id and thus correct actor to assign it the stream.

- `Joined Room`: This event is called once we join the room ourselves. Here we can create an Audio Capture and from it an ODIN Media Stream that we add to the room afterwards via the `Add Media` node. We also save the Audio Captures of both rooms in a variable to reference them later.

- `Peer Left`: This is called when another peer leaves. Here we just print a message in this sample.

- `Media Added`: This Event is called when the media stream of another peer is added to the room. It grabs the actors map from our Networking Component and adds an `Odin Synth Component`to it, activates it and assigns it the given media stream.

Additionally we have some functionality to adjust the APM Settings of the rooms - this is done in the `SetApmSettings` event. Also we can open the APM Settings menu with the `OpenOptions` event and close them with `CloseOptions`.

Lastly, in the `Tick` Event we start and stop the Capturing of the Proximity Chat and Radio Chat Audio Captures - depending on whether we press the **R Key** or not. The `RPressed` variable is set from the player controller class.

![OdinClientComponentTick.png.png](/Documentation/img/OdinClientComponentTick.png)

## Odin Fleet Online Subsystem

As mentioned before, the project uses a custom Online Subsystem (OSS) to connect to the running dedicated server in [Odin Fleet](https://odin.4players.io/fleet/). The `OdinFleet` OSS implements all necessary classes and functions in a minimal manner to enable session discovery, join and destruction. Thus, the interesting functions live inside the `IOnlineSession`-Interface, implemented in the `FOnlineSessionOdinFleet` class. `FindSessions` starts an `HTTP GET` request to a custom [Azure Functions REST API](https://azure.microsoft.com/en-us/products/functions) that returns the IP Address and open Unreal Port of the dedicated server in Odin Fleet. While the function runs on an Azure instance, its code can be found in the `AzureBackend` folder of this repository to illustrate the necessary steps to retrieve server information from Odin Fleet: The Azure Function starts an `HTTP GET` request to the Odin Fleet REST API, with proper authentication and an App ID, and receives the IP Address and Port of the first running Server of that App.

:warning: Using a custom OSS to connect to a dedicated server is not mandatory, but can be good praxis because you can simply switch out the OSS. This is just used for illustration purposes in this demo, but you can as well use a Blueprint only approach - ultimately opening the connection e.g. with the [Open Level](https://dev.epicgames.com/documentation/en-us/unreal-engine/BlueprintAPI/Game/OpenLevel_byName) Node and using the IP Address and Port of the dedicated server.

Once this information is returned to the Unreal Client, it will start to Join the Session using the `JoinSession` function of the `FOnlineSessionOdinFleet` class. Once that is done, the client's Game Instance (implemented in the `MyGameInstance` class) retrieves an event and triggers its `OnJoinSessionComplete` routine, calling `APlayerController->ClientTravel()` on the local player controller, using the retrieved IP Address and Port. Now the session is handled like any Unreal Networking session using UDP/IP for communication.

### Hosting an own instance of the demo

You can duplicate the setup of the demo in your own infrastructure - e.g. when you wish a closed demonstration of Odin. To run your own instance you need to run a dedicated server in Odin Fleet, create an own instance of the Azure Functions Backend and finally update the URL that the OdinFleet OSS uses for its HTTP Requests. This section will go through each of these steps.

#### 1. Run a Dedicated Game Server on Odin Fleet

To run a dedicated game server on Odin Fleet you first need to create a [Docker Image](https://www.docker.com/) - e.g. on [Docker Hub](https://hub.docker.com/). To get started, you can build the image locally, using [Docker Desktop](https://www.docker.com/), and configure it for Linux Containers (this is also possible on a Windows Machine, [as described here](https://learn.microsoft.com/en-us/virtualization/windowscontainers/deploy-containers/set-up-linux-containers)) and optionally sign up or login to [Docker Hub](https://hub.docker.com/). You can use the packaged build from the [most recent release](https://github.com/4Players/odin-unreal-demo/releases/latest). Once downloaded, unpack it and place a Dockerfile next to it, you can use the [example dockerfile in this repository](https://github.com/4Players/odin-unreal-demo/blob/main/dockerfile) as a reference. Now, open a Shell, or Command Prompt respectively, and navigate to the folder containing the unpacked LinuxServer-Package and the dockerfile. Here you can now build, tag and push your docker container image. If you did not login to Dockerhub, or want to use another container registry, you need to follow steps specific to the registry in order to authenticate before pushing the container image.

```
docker build -t odin-unreal-demo-linux-server .
docker tag odin-unreal-demo-linux-server <your-registry>/odin-unreal-demo-linux-server:latest
docker push <your-registry>/odin-unreal-demo-linux-server
```
To test the image, you can also run the image locally, 17778 can be any free IP port of your local machine:

```
docker run -p 17778:7777/udp --name unreal-demo-test -d odin-unreal-demo-linux-server
```

Now you can start the Unreal Client and connect directly to the VM using Command Line Options when starting, navigate to the folder with the executable in your Command Prompt or Shell and then:

```
[GameExecutable] 127.0.0.1:17778
```
This should now open the game's level instead of lobby menu and you can run around. If you want start a second client and see if they can now see each other. If everything is fine you can proceed to setup the Server in Odin Fleet.

After the image is pushed, you can now use it in Odin Fleet. Follow the [Getting Started Guide](https://docs.4players.io/fleet/guides/getting-started/) to create an Account, add an Application and reference the container image from your registry that you just created. Next, create a Server Configuration - make sure to open a Port for the Unreal Communication in the Advanced Server Settings. For the example dockerfile it should be Port 7777 and Protocol should be set to UDP. Lastly create a Server Deployment of your choice from the configuration to start the server. You now have a running instance of your Odin Unreal Sample's dedicated server executable!

#### 2. Run an Azure Function App

Next, we need to create and configure an Azure Function App to run the backend code that communicates with the Odin Fleet API. For this step, you need a Microsoft Azure Account, [you can start here](https://azure.microsoft.com/en-us/Free) to use the Azure Portal.  For this, download or clone the repository's code and build it, e.g. using [Visual Studio](https://visualstudio.microsoft.com/). You can use the Visual Studio IDE to deploy the function app directly to your account (creating a new function app or deploying to an existing one), or you can use another method, if needed, like [Zip Deployment for Azure Functions](https://learn.microsoft.com/en-us/azure/azure-functions/deployment-zip-push).

Once the App runs on your Azure Account, you need to add two Environment Variables to your Function App's configuration. See [the Microsoft Documentation](https://learn.microsoft.com/en-us/azure/azure-functions/functions-how-to-use-azure-function-app-settings?tabs=azure-portal%2Cto-premium) for a detailed guide on how to set Environment Variables in your Apps. Fetch the App ID of your Odin Fleet App in the [4Players Console](https://console.4players.io/) and create a Personal Access Token in the Settings Section of the App. Set these as the respective Environment Variables of your Function App:

```
"ODIN_APP_ID": "<your app id>",
"ODIN_API_TOKEN": "<your personal access token>"
```

Now the Azure Function App is running and can authenticate when accessing the Odin Fleet API!

#### 3. Update OSS URL

Lastly, we need to adjust the URL that is called when trying to find sessions. Retrieve the URL from your Function App and copy it in the Main Menu's settings to the `Backend URL` textbox. Now, once you `Join` in the Main Menu, the OdinFleet OSS will ask the alternative Backend. This is saved to a json-file inside the packaged game folder, so when you distribute the packaged game to the other players, make sure to copy the json file with it, so that they do not need to re-configure.

## Conclusion

This was a rough rundown of the demo project - to get started in your own project with ODIN you can copy and paste any portions of the project. Also have a look at the [ODIN Unreal SDK Manual](https://docs.4players.io/voice/unreal/manual/). Here you have a step by step guide on how to implement the ODIN SDK in your application. On the [4Players Website](https://odin.4players.io/voice-chat/) you get all information on how to integrate ODIN properly.
