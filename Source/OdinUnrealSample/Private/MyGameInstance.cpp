// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Components/AudioComponent.h"
#include "AudioDevice.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OdinAudio/OdinAudioCapture.h"
#include "OdinAudio/OdinDecoder.h"
#include "OdinAudio/OdinEncoder.h"
#include "OdinAudio/OdinSynthComponent.h"
#include "OdinRoom.h"
#include "OdinRoomCallbackComponent.h"
#include "OdinSubsystem.h"
#include "UObject/UObjectIterator.h"


void UMyGameInstance::Init()
{
    Super::Init();

    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (OnlineSubsystem)
    {
        IOnlineIdentityPtr Identity = OnlineSubsystem->GetIdentityInterface();
        if (Identity.IsValid())
        {
            Identity->AddOnLoginCompleteDelegate_Handle(0, FOnLoginCompleteDelegate::CreateUObject(this, &UMyGameInstance::OnLoginComplete));
        }

    	IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
        if (Session.IsValid())
        {
		    Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UMyGameInstance::OnJoinSessionComplete));
        }
    }

    WorldBeginTearDownHandle = FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &UMyGameInstance::HandleWorldBeginTearDown);
}

void UMyGameInstance::Shutdown()
{
    CleanupOdinBeforeTravel(TEXT("GameInstanceShutdown"));

    if (WorldBeginTearDownHandle.IsValid())
    {
        FWorldDelegates::OnWorldBeginTearDown.Remove(WorldBeginTearDownHandle);
        WorldBeginTearDownHandle.Reset();
    }

    Super::Shutdown();
}

void UMyGameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
    if (!bWasSuccessful) return;

    UWorld* World = GetWorld();
    if (!World) return;

    APlayerController* PC = UGameplayStatics::GetPlayerController(World, LocalUserNum);
    if (PC && PC->PlayerState)
    {
        PC->PlayerState->SetUniqueId(UserId);
    }
}

void UMyGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (Result != EOnJoinSessionCompleteResult::Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to join session: %s"), *SessionName.ToString());
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Successfully joined session: %s"), *SessionName.ToString());

    UWorld* World = GetWorld();
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (!World || !OnlineSubsystem)
    {
        return;
    }

    IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    if (!Session.IsValid() || !PlayerController || !PlayerController->PlayerState)
    {
        return;
    }

    FString ConnectString;
    if (Session->GetResolvedConnectString(SessionName, ConnectString, NAME_GamePort))
    {
        PlayerController->ClientTravel(ConnectString, TRAVEL_Absolute);
    }
}

void UMyGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
    Super::OnWorldChanged(OldWorld, NewWorld);

    if (NewWorld && NewWorld->GetGameInstance() == this)
    {
        bOdinCleanupTriggered = false;

        if (!RetiredOdinCaptures.IsEmpty() || !RetiredOdinEncoders.IsEmpty())
        {
            NewWorld->GetTimerManager().SetTimer(
                RetiredOdinReleaseTimer, this, &ThisClass::ReleaseRetiredOdinObjects, 1.0f, false);
        }
    }
}

void UMyGameInstance::HandleWorldBeginTearDown(UWorld* World)
{
    if (!World || World->GetGameInstance() != this)
    {
        return;
    }

    CleanupOdinBeforeTravel(TEXT("WorldBeginTearDown"), World);
}

void UMyGameInstance::ReleaseRetiredOdinObjects()
{
    int32 FreedEncoders = 0;
    for (UOdinEncoder* Encoder : RetiredOdinEncoders)
    {
        if (IsValid(Encoder) && UOdinEncoder::FreeEncoder(Encoder))
        {
            ++FreedEncoders;
        }
    }
    RetiredOdinEncoders.Reset();

    const int32 ReleasedCaptures = RetiredOdinCaptures.Num();
    RetiredOdinCaptures.Reset();
    UE_LOG(LogTemp, Log, TEXT("ODIN retired object cleanup: released %d captures and freed %d encoders."),
        ReleasedCaptures, FreedEncoders);
}

void UMyGameInstance::CleanupOdinBeforeTravel(const TCHAR* Reason, UWorld* WorldToClean)
{
    if (bOdinCleanupTriggered)
    {
        return;
    }

    UWorld* CurrentWorld = WorldToClean ? WorldToClean : GetWorld();
    if (!CurrentWorld || CurrentWorld->GetGameInstance() != this)
    {
        return;
    }

    bOdinCleanupTriggered = true;

    int32 DisabledRoomCallbackComponents = 0;
    for (TObjectIterator<UOdinRoomCallbackComponent> It; It; ++It)
    {
        UOdinRoomCallbackComponent* CallbackComponent = *It;
        if (!IsValid(CallbackComponent) || CallbackComponent->HasAnyFlags(RF_ClassDefaultObject)
            || CallbackComponent->GetWorld() != CurrentWorld)
        {
            continue;
        }

        CallbackComponent->PrepareForWorldTeardown();
        ++DisabledRoomCallbackComponents;
    }

    int32 StoppedCaptures = 0;
    for (TObjectIterator<UOdinAudioCapture> It; It; ++It)
    {
        UOdinAudioCapture* Capture = *It;
        if (!IsValid(Capture) || Capture->HasAnyFlags(RF_ClassDefaultObject) || Capture->GetWorld() != CurrentWorld)
        {
            continue;
        }

        Capture->StopCapturingAudio();
        RetiredOdinCaptures.AddUnique(Capture);
        ++StoppedCaptures;
    }

    int32 UnlinkedEncoders = 0;
    for (TObjectIterator<UOdinEncoder> It; It; ++It)
    {
        UOdinEncoder* Encoder = *It;
        if (!IsValid(Encoder) || Encoder->HasAnyFlags(RF_ClassDefaultObject) || Encoder->GetWorld() != CurrentWorld)
        {
            continue;
        }

        if (UOdinSubsystem* OdinSubsystem = UOdinSubsystem::Get())
        {
            OdinSubsystem->UnlinkEncoder(Encoder);
            ++UnlinkedEncoders;
        }

        // A grace period lets any in-flight native callback finish before the UObject is released.
        RetiredOdinEncoders.AddUnique(Encoder);
    }

    int32 ClosedRooms = 0;
    for (TObjectIterator<UOdinRoom> It; It; ++It)
    {
        UOdinRoom* Room = *It;
        if (!IsValid(Room) || Room->HasAnyFlags(RF_ClassDefaultObject))
        {
            continue;
        }

        if (Room->GetWorld() != CurrentWorld)
        {
            continue;
        }

        if (Room->IsConnected())
        {
            Room->CloseRoom();
            ++ClosedRooms;
        }
    }

    TArray<UOdinSynthComponent*> SynthsToDestroy;
    for (TObjectIterator<UOdinSynthComponent> It; It; ++It)
    {
        UOdinSynthComponent* SynthComponent = *It;
        if (!IsValid(SynthComponent) || SynthComponent->HasAnyFlags(RF_ClassDefaultObject))
        {
            continue;
        }

        if (SynthComponent->GetWorld() != CurrentWorld)
        {
            continue;
        }

        if (UAudioComponent* AudioComponent = SynthComponent->GetConnectedAudioComponent())
        {
            AudioComponent->bAllowSpatialization = false;
            AudioComponent->Stop();
            AudioComponent->Deactivate();
        }

        SynthComponent->SetComponentTickEnabled(false);
        SynthComponent->Stop();
        SynthComponent->Deactivate();
        SynthsToDestroy.Add(SynthComponent);
    }

    TArray<UOdinDecoder*> DecodersToFree;
    for (TObjectIterator<UOdinDecoder> It; It; ++It)
    {
        UOdinDecoder* Decoder = *It;
        if (!IsValid(Decoder) || Decoder->HasAnyFlags(RF_ClassDefaultObject))
        {
            continue;
        }

        if (Decoder->GetWorld() != CurrentWorld)
        {
            continue;
        }

        DecodersToFree.Add(Decoder);
    }

    // Stop commands must reach the mixer before components used by Steam Audio are destroyed.
    if (FAudioDevice* AudioDevice = CurrentWorld->GetAudioDeviceRaw())
    {
        AudioDevice->Flush(CurrentWorld);
    }

    for (UOdinSynthComponent* SynthComponent : SynthsToDestroy)
    {
        if (IsValid(SynthComponent))
        {
            SynthComponent->DestroyComponent();
        }
    }

    int32 FreedDecoders = 0;
    for (UOdinDecoder* Decoder : DecodersToFree)
    {
        if (IsValid(Decoder) && UOdinDecoder::FreeDecoder(Decoder))
        {
            ++FreedDecoders;
        }
    }

    UE_LOG(LogTemp, Log,
        TEXT("ODIN pre-travel cleanup (%s): disabled %d callback components, stopped %d captures, unlinked %d encoders, closed %d rooms, stopped %d synths, freed %d decoders."),
        Reason, DisabledRoomCallbackComponents, StoppedCaptures, UnlinkedEncoders, ClosedRooms,
        SynthsToDestroy.Num(), FreedDecoders);
}
