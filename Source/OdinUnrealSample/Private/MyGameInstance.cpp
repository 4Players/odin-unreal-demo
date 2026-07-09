// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include "OdinAudio/OdinDecoder.h"
#include "OdinAudio/OdinSynthComponent.h"
#include "OdinRoom.h"
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

    PreLoadMapHandle = FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UMyGameInstance::HandlePreLoadMap);
    PostLoadMapHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMyGameInstance::HandlePostLoadMap);
    WorldBeginTearDownHandle = FWorldDelegates::OnWorldBeginTearDown.AddUObject(this, &UMyGameInstance::HandleWorldBeginTearDown);
}

void UMyGameInstance::Shutdown()
{
    CleanupOdinBeforeTravel(TEXT("GameInstanceShutdown"));

    if (PreLoadMapHandle.IsValid())
    {
        FCoreUObjectDelegates::PreLoadMap.Remove(PreLoadMapHandle);
        PreLoadMapHandle.Reset();
    }

    if (PostLoadMapHandle.IsValid())
    {
        FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(PostLoadMapHandle);
        PostLoadMapHandle.Reset();
    }

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

void UMyGameInstance::HandlePreLoadMap(const FString& MapName)
{
    CleanupOdinBeforeTravel(TEXT("PreLoadMap"));
}

void UMyGameInstance::HandlePostLoadMap(UWorld* LoadedWorld)
{
    bOdinCleanupTriggered = false;
}

void UMyGameInstance::HandleWorldBeginTearDown(UWorld* World)
{
    CleanupOdinBeforeTravel(TEXT("WorldBeginTearDown"), World);
}

void UMyGameInstance::CleanupOdinBeforeTravel(const TCHAR* Reason, UWorld* WorldToClean)
{
    if (bOdinCleanupTriggered)
    {
        return;
    }

    UWorld* CurrentWorld = WorldToClean ? WorldToClean : GetWorld();
    if (!CurrentWorld)
    {
        return;
    }

    bOdinCleanupTriggered = true;

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

    int32 StoppedSynths = 0;
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
        SynthComponent->DestroyComponent();
        ++StoppedSynths;
    }

    int32 FreedDecoders = 0;
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

        if (UOdinDecoder::FreeDecoder(Decoder))
        {
            ++FreedDecoders;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("ODIN pre-travel cleanup (%s): closed %d rooms, stopped %d synths, freed %d decoders."),
        Reason, ClosedRooms, StoppedSynths, FreedDecoders);
}
