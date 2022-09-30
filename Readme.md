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

* The sample is built with the Unreal Engine version 4.27, you may need to adjust the project in order to make it fully functional in UE 5.0
* Although it builts upon Photon's networking engine, the sample delivers all needed libraries for that
* The sample depends on Steam Audio, which is delivered together with the Unreal Engine so you do not need to install the plugin manually, if you have installed plugins with your Unreal Engine version
* ## Version History
* 0.1
  * Initial Release

## Project Structure

*This guide is written for Unreal Engine version 4.27 as the project is targeted for this version of the engine. You will need to adjust for Unreal Engine 5.0 and above accordingly.*

The project is based on the **Top Down Example** by Epic Games, so most of its structure can be found in this project as well. The code is based on the **Unreal Example of Photon** with adjustments so that it works for a 3D Game with continuous movement and integrates in the project structure properly.

This document outlines the structure of this sample project and describes how you can achieve similar results in your Unreal project. First we will have a look at the overall folder structure of the project so you can find every asset. Next we will outline the Game Mode, Game Instance and Player Controller classes to create the basic game rules and input methods. After that we describe the Photon Networking of this project and how it replicates the player characters' positions to all clients before we look at how we incorporate Odin here so that each character gets the correct Odin Media assigned. Lastly we look at the map and see what features of the Unreal Audio Engine are show cased here in which way.

### Source Code

As stated, the source code is mostly based on the [Unreal Photon Example](https://doc.photonengine.com/en-us/realtime/current/getting-started/unreal-engine). To get a basic understanding of what they did to integrate Photon, check out their documentation on the sample. We have integrated the Content of the Top Down Example by Epic Games and made some minor adjustments to the Photon Code.

Firstly, we have changed the `UPhotonLBClient` - the heart and meat of the Photon integration - to a `UActorComponent` instead of an `AActor`. As a consequence we have changed some overridden functions: `Tick` becomes `TickComponent` and needs some additional parameters. Additionally, in the constructor we set `PrimaryComponentTick.bCanEverTick = true;`. Once this is done, we can inherit an Actor Component from this class to more easily integrate it in our Project Structure.

Secondly, we have changed the `SetLocalPlayerPos` to take 6 instead of 2 `float`s as inputs. We adjusted the `LoadBalancingListener::setLocalPlayerPos()` accordingly to send these floats as well and the `LoadBalancingListener::customEventAction()` function needs to handle the new kind of object as well. Have a look at the `LoadBalancingListener.cpp` and the `PhotonLBClient.h` and `PhotonLBClient.cpp` files in the project to get the complete source code of these adjustments. The rest of the source code stayed as it was in the example.

### Content Directory

Most of the project's logic is located inside the `Blueprints` folder. Other folders hold the needed fonts for texts, meshes, materials and so on. In the `Sound` folder you can find the used attenuation settings and sound effect assets. Depending on which audio engine you whish to use they may differ from what you need in your project. In order to get an idea of what they do and how the settings affect the audio output of the project you can check out the documentation on [Sound in General in the Unreal Engine](https://docs.unrealengine.com/4.27/en-US/WorkingWithAudio/), and [Sound Attenuation in the Unreal Engine](https://docs.unrealengine.com/4.27/en-US/WorkingWithAudio/DistanceModelAttenuation/) specifically as well as [Submixes](https://docs.unrealengine.com/4.27/en-US/WorkingWithAudio/Submixes/). The [Steam Audio Unreal Engine Integration](https://valvesoftware.github.io/steam-audio/doc/unreal/index.html) has a pretty helpful documentation on their SDK as well.

The `Maps` folder holds both the map for the Lobby UI and the default map of the project. The `Lobby` map simply overrides the standard Game Mode with a Lobby Game Mode that opens the needed UI. The main map is the `TopDownExampleMap` in here.

In the Blueprint Folder you can find the Game Instance, Game Mode, Player Controller and Player Character Classes - these are the entry points for the engine to our game logic, so we will have a look at them in the next paragraph of this guide.

Next to them you can find folders for each different kind of objects in the project - `Interactive Objects` holds an Actor for the playback of a simple sound that uses the same attenuation settings as the Odin integration so that you can compare the Odin Audio to the internal sounds directly produced in the engine. In `Lobby` you can find an additional Game Mode for the Lobby Menu that simply loads the corresponding UI. In `Networking` you can find everything related to Photon. `Odin` holds the custom Actor Component we have built for this project as well as some helper classes. Lastly, in the `Widgets` folder you can find any widget that is used in the project - they are built in a way that they do not need to know of the Odin SDK, so that you do not have to look into these classes. Of course it can make sense for you to copy and paste widgets from the `Options Menu` for use in your own game as the needed UI for the Odin Options stays the same for each application.

![ProjectFolderStructure.png](/Documentation/img/ProjectFolderStructure.png)

Now that you have an overview of the Project in general, we can have a look at the Standard Game Mode and its related classes.

### Game Mode, Game Instance and Player Controller

The Game Mode `Top Down Game Mode` used in the games main map has almost no logic in it and only defines the used default classes  - we have changed the default player controller to the `TopDownController` class and the default pawn class to `TopDownCharacter`. 

![GameModeSettings.png](/Documentation/img/GameModeSettings.png)

In the project settings (in the main menu bar go to `Edit` and then `Project Settings...`) then we needed to change the game instance class to `GameInstance_TD` in the Maps&Modes category. 

![GameInstance.png](/Documentation/img/GameInstance.png)

You can also just change your default GameMode here or you can define it on a per-map basis which we did in this project. To do so, open the map, go to `Blueprints` above the renderer panel and select your game mode under `World Overrides -> Game Mode`.

![MapOverrideGameMode.png](/Documentation/img/MapOverrideGameMode.png)

The Game Instance has no logic as well - it just holds a string variable `Player Name` that the player may choose in the Lobby and that needs to be saved when loading the main map. Here it is used to identify a player's character on other clients - so we will just copy the player name and replicate it to the other clients. We will do so in a minute.

But first we will have a look at the `Top Down Controller`. The logic in here fully relates to inputs - they allow switching between a top down paradigm to first person perspective and back at any time to help play around with the immersion coming from the 3D Audio.

More important for the Controller are the assigned Components. The `PhotonLBClient_TD` is our networking component that we will use to replicate player's movements, `OdinClientComponent` is the custom Component handling all the logic for the communication with Odin and `UiManager` simply handles some of the UI shown on screen - in our case it just adds and removes widgets reflecting connected players.

![ControllerComponents.png](/Documentation/img/ControllerComponents.png)

In the next part we will now look at what the Networking Component does exactly.

### Photon Component and Networking

The `PhotonLBClient_TD` class can be found in the `Blueprints/Networking` folder. It inherits from the `UPhotonLBClient` Actor Component from the C++ Source Code and thus already has most functionality to communicate with the Photon framework. The Component extends the class to handle our use case - yet it is still agnostic to the Odin Framework and thus implements the `Networking Component` Blueprint Interface so that other Components can communicate with the implemented networking framework without knowing which one is used exactly. You can easily extend or exchange this component for anything you would like to use in your application.

Just like the Source Code, the blueprints of this class are based on the sample blueprints of the Photon sample for the Unreal Engine. It has Events that are called from the C++ parent class when a player joins the Photon room, when one player leaves it, 


![PhotonComponentClassSettings.png](/Documentation/img/PhotonComponentClassSettings.png)

### Odin Component

### Sample Map

## Conclusion
