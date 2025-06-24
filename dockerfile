# Copy the pre-packaged files into a minimal container image
FROM gcr.io/distroless/cc-debian10:nonroot

# Copy dedicated server files
COPY --chown=nonroot:nonroot OdinUnrealDemo_1.8.0_LinuxServer/ /home/nonroot/server/

# Expose default UE server port
EXPOSE 7777/udp

# Launch the server
CMD ["/home/nonroot/server/OdinUnrealSample/Binaries/Linux/OdinUnrealSampleServer", "/Game/Maps/TopDownExampleMap", "-log", "-unattended", "-port=7777"]