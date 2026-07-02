FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        ca-certificates \
        libcurl4 \
        libgcc-s1 \
        libicu70 \
        libnss3 \
        libssl3 \
        libstdc++6 \
        zlib1g \
    && rm -rf /var/lib/apt/lists/*

RUN useradd -m -u 10001 nonroot

COPY --chown=nonroot:nonroot Build/Artifacts/Linux/Server/ /home/nonroot/server/

USER nonroot

EXPOSE 7777/udp

CMD ["/home/nonroot/server/OdinUnrealSample/Binaries/Linux/OdinUnrealSampleServer", "/Game/Maps/TopDownExampleMap", "-log", "-unattended", "-port=7777"]
