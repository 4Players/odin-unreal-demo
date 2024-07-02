# Odin Unreal Demo

This is a simple demonstration of the usage of the [Unreal SDK](https://github.com/4Players/odin-sdk) of 4Player's ODIN, a Voice Chat full service solution. The SDK provides an audio stream that can be processed in the audio engine of Unreal to add spatialization or audio effects.

This version is built upon the [Unreal Engine's Networking Solution](https://docs.unrealengine.com/4.27/en-US/InteractiveExperiences/Networking/Overview/). This sample works for Unreal Engine 5.3+.

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

In the releases you can find a pre-built game executable and the current project's code base.  The pre-built game can likely be executed without any installations required - if you do not have the redistributable packages of C++ installed, that Unreal needs, you can find it in the `\Engine\Extras\Redist\en-us`folder of the pre-built game.

To open the project in the Unreal Editor you need to install the Unreal Engine 5.3 or higher. You can open the source code with the corresponding version of the branch easily. If you need to open it with a higher version of the Unreal Engine you can right-click the `OdinUnrealSample.uproject` file and `Switch Unreal Engine version ...` to the Engine version you have installed that you want the project to open with. If the Editor fails to launch, rebuild the project from Visual Studio. You might also need to exchange the Odin Plugin to the corresponding version, downloadable e.g. in its [Github Repository](https://github.com/4Players/odin-sdk-unreal/releases).

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

*This guide is written for Unreal Engine version 5.3 as the project is targeted for this version of the engine. You will need to adjust for Unreal Engine 5.4 and above accordingly.*

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

The `Start Connect` Custom Event is called once the local player is connected to the Photon room. It is mainly taken from the [ODIN Unreal SDK Manual](https://www.4players.io/odin/sdk/unreal/manual/). In summary, we generate a room token with our access key, a room id and a user id. Note, that the Access Key should not be known to the client normally and you should do this on a trusted Web Server instead. For the simplicity of this sample we kept the call on the client though.

Then we construct a room and bind all needed events to appropriate Custom Events (we go through them in a bit). Then we create some User Data - they contain the chosen User Name from the Login Screen and the Network Id from our Photon Component. The Network Id is needed by the other clients so that they can get the correct Player Character from the Photon Component to assign it an Odin Synth Component with the correct Media Stream. Once that is done, we will call the `Join Room` function and are done with the event. We do this two times in total, once for each Odin Room.

We handle the Room Events in different events:

- `Peer Joined`: This event is called before the media of the other peer is added to the room, so we can setup the player character - we use the network id in the user data and the passed ODIN peer id to create a map on this component, so once we get the media stream with the ODIN id we can get the correct network id and thus correct actor to assign it the stream.

- `Joined Room`: This event is called once we join the room ourselves. Here we can create an Audio Capture and from it an ODIN Media Stream that we add to the room afterwards via the `Add Media` node. We also save the Audio Captures of both rooms in a variable to reference them later.

- `Peer Left`: This is called when another peer leaves. Here we just print a message in this sample.

- `Media Added`: This Event is called when the media stream of another peer is added to the room. It grabs the actors map from our Networking Component and adds an `Odin Synth Component`to it, activates it and assigns it the given media stream.

Additionally we have some functionality to adjust the APM Settings of the rooms - this is done in the `SetApmSettings` event. Also we can open the APM Settings menu with the `OpenOptions` event and close them with `CloseOptions`.

Lastly, in the `Tick` Event we start and stop the Capturing of the Proximity Chat and Radio Chat Audio Captures - depending on whether we press the **R Key** or not. The `RPressed` variable is set from the player controller class.

![OdinClientComponentTick.png.png](/Documentation/img/OdinClientComponentTick.png)

## Conclusion

This was a rough rundown of the demo project - to get started in your own project with ODIN you can copy and paste any portions of the project. Also have a look at the [ODIN Unreal SDK Manual](https://www.4players.io/odin/sdk/unreal/manual/). Here you have a step by step guide on how to implement the ODIN SDK in your application. On the [4Players Website](https://www.4players.io/products/voice-chat/) you get all information on how to integrate ODIN properly.
