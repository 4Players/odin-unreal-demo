# Odin Unreal Demo

This is a simple demonstration of the usage of the [Unreal SDK](https://github.com/4Players/odin-sdk) of 4Player's Odin, a Voice Chat service. The SDK provides an audio stream that can be processed in the audio engine of Unreal to add spatialization or audio effects.

This version is built upon the free tier of the [Photon Networking Engine](https://doc.photonengine.com/en-us/realtime/current/getting-started/unreal-engine). The audio stream is processed with the help of [Steam Audio](https://valvesoftware.github.io/steam-audio/), which is delivered together with the Unreal Engine.

## Features

The Demo showcases the usage of Odin together with Unreal's Audio Engine. The example uses two Odin voice chat rooms - one for spatialized (proximity) chat, and one for a radio simulation with audio effects applied. It shows:

- how to connect to an Odin voice chat room with an acquired access token
- how to attach spatialization to the incoming audio stream, using an audio engine plugin like Steam Audio
- how to send the voice chat audio stream to audio effect processors

## Getting Started

In the releases you can find a pre-built game executable and the current project's code base.  The pre-built game can likely be executed without any installations required - if you do not have the redistributable packages of C++ installed, that Unreal needs, you can find it in the `\Engine\Extras\Redist\en-us`folder of the pre-built game.

To open the project in the Unreal Editor you need to install the Unreal Engine 4.27.

### Dependencies

* The sample is built with the Unreal Engine version 4.27
* Although it builts upon Photon's networking engine, the sample delivers all needed libraries for that
* The sample depends on Steam Audio, which is delivered together with the Unreal Engine so you do not need to install the plugin manually, if you have installed plugins with your Unreal Engine version
* ## Version History
* 0.1
  * Initial Release
