// Fill out your copyright notice in the Description page of Project Settings.

#include "OdinRoomCallbackComponent.h"

#include "Components/AudioComponent.h"
#include "OdinAudio/OdinDecoder.h"
#include "OdinAudio/OdinSynthComponent.h"
#include "OdinFunctionLibrary.h"
#include "OdinRoom.h"
#include "Engine/World.h"
#include "TimerManager.h"

UOdinRoomCallbackComponent::UOdinRoomCallbackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOdinRoomCallbackComponent::BeginPlay()
{
	Super::BeginPlay();
	bOdinTeardownStarted = false;

	for (UOdinRoom* Room : AutoBindRooms)
	{
		BindRoomCallbacks(Room);
	}
}

void UOdinRoomCallbackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindAllRoomCallbacks();
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(AudioCleanupTimer);
	}
	FlushPendingAudioCleanup();
	Super::EndPlay(EndPlayReason);
}

void UOdinRoomCallbackComponent::BindRoomCallbacks(UOdinRoom* Room)
{
	if (bOdinTeardownStarted || !IsValid(Room) || BoundRooms.Contains(Room))
	{
		return;
	}

	Room->OnRoomPeerLeftBP.AddUniqueDynamic(this, &ThisClass::HandleRoomPeerLeft);
	Room->OnRoomStatusChangedBP.AddUniqueDynamic(this, &ThisClass::HandleRoomStatusChanged);
	BoundRooms.Add(Room);
}

void UOdinRoomCallbackComponent::UnbindRoomCallbacks(UOdinRoom* Room)
{
	if (!IsValid(Room))
	{
		return;
	}

	Room->OnRoomPeerLeftBP.RemoveDynamic(this, &ThisClass::HandleRoomPeerLeft);
	Room->OnRoomStatusChangedBP.RemoveDynamic(this, &ThisClass::HandleRoomStatusChanged);
	BoundRooms.Remove(Room);
}

void UOdinRoomCallbackComponent::UnbindAllRoomCallbacks()
{
	const TArray<TObjectPtr<UOdinRoom>> RoomsToUnbind = BoundRooms;
	for (UOdinRoom* Room : RoomsToUnbind)
	{
		UnbindRoomCallbacks(Room);
	}
}

int32 UOdinRoomCallbackComponent::CleanupPeerAudio(UOdinRoom* Room, int64 PeerId, bool bDestroySynthComponents, bool bFreeDecoders)
{
	if (bOdinTeardownStarted || !IsValid(Room))
	{
		return 0;
	}

	int32 CleanedObjects = 0;
	const TArray<UOdinDecoder*> Decoders = UOdinFunctionLibrary::GetDecodersForPeer(Room, PeerId);
	for (UOdinDecoder* Decoder : Decoders)
	{
		CleanedObjects += CleanupDecoderAudio(Decoder, bDestroySynthComponents, bFreeDecoders);
	}

	return CleanedObjects;
}

int32 UOdinRoomCallbackComponent::CleanupDecoderAudio(UOdinDecoder* Decoder, bool bDestroySynthComponents, bool bFreeDecoder)
{
	if (bOdinTeardownStarted || !IsValid(Decoder))
	{
		return 0;
	}

	if (PendingDecoderCleanup.Contains(Decoder))
	{
		return 0;
	}

	int32 CleanedObjects = 0;
	if (bDestroySynthComponents)
	{
		TArray<UOdinSynthComponent*> SynthsToDestroy;
		for (TObjectIterator<UOdinSynthComponent> It; It; ++It)
		{
			UOdinSynthComponent* SynthComponent = *It;
			if (!IsValid(SynthComponent) || SynthComponent->HasAnyFlags(RF_ClassDefaultObject))
			{
				continue;
			}

			if (SynthComponent->GetDecoder() != Decoder)
			{
				continue;
			}

			SynthsToDestroy.Add(SynthComponent);
		}

		for (UOdinSynthComponent* SynthComponent : SynthsToDestroy)
		{
			if (UAudioComponent* AudioComponent = SynthComponent->GetConnectedAudioComponent())
			{
				AudioComponent->bAllowSpatialization = false;
				AudioComponent->Stop();
				AudioComponent->Deactivate();
			}

			SynthComponent->SetComponentTickEnabled(false);
			SynthComponent->Stop();
			SynthComponent->Deactivate();
			PendingSynthCleanup.AddUnique(SynthComponent);
			++CleanedObjects;
		}
	}

	if (bFreeDecoder)
	{
		PendingDecoderCleanup.Add(Decoder);
		++CleanedObjects;
	}

	if (UWorld* World = GetWorld(); World && !World->GetTimerManager().IsTimerActive(AudioCleanupTimer))
	{
		World->GetTimerManager().SetTimer(AudioCleanupTimer, this, &ThisClass::FlushPendingAudioCleanup, AudioCleanupGracePeriod, false);
	}
	else if (!GetWorld())
	{
		FlushPendingAudioCleanup();
	}

	return CleanedObjects;
}

void UOdinRoomCallbackComponent::PrepareForWorldTeardown()
{
	if (bOdinTeardownStarted)
	{
		return;
	}

	bOdinTeardownStarted = true;
	UnbindAllRoomCallbacks();
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(AudioCleanupTimer);
	}

	// MyGameInstance owns the full-world cleanup from this point; do not free the same objects twice.
	PendingSynthCleanup.Reset();
	PendingDecoderCleanup.Reset();
}

void UOdinRoomCallbackComponent::FlushPendingAudioCleanup()
{
	for (UOdinSynthComponent* SynthComponent : PendingSynthCleanup)
	{
		if (IsValid(SynthComponent))
		{
			SynthComponent->DestroyComponent();
		}
	}
	PendingSynthCleanup.Reset();

	for (UOdinDecoder* Decoder : PendingDecoderCleanup)
	{
		if (IsValid(Decoder))
		{
			UOdinDecoder::FreeDecoder(Decoder);
		}
	}
	PendingDecoderCleanup.Reset();
}

void UOdinRoomCallbackComponent::HandleRoomPeerLeft(UOdinRoom* Room, FOdinPeerLeft Data)
{
	if (bOdinTeardownStarted)
	{
		return;
	}

	const int32 CleanedObjects = CleanupPeerAudio(Room, Data.peer_id);
	UE_LOG(LogTemp, Log, TEXT("ODIN peer-left cleanup: room=%s, peer=%lld, scheduled objects=%d."),
		*GetNameSafe(Room), Data.peer_id, CleanedObjects);
	OnOdinPeerLeft(Room, Data);
}

void UOdinRoomCallbackComponent::HandleRoomStatusChanged(UOdinRoom* Room, FOdinRoomStatusChanged Data)
{
	OnOdinRoomStatusChanged(Room, Data);

	if (Data.status == FOdinRoomStatusChanged::ClosedStatus)
	{
		OnOdinRoomClosed(Room, Data);
	}
}

void UOdinRoomCallbackComponent::OnOdinPeerLeft_Implementation(UOdinRoom* Room, const FOdinPeerLeft& Data)
{
}

void UOdinRoomCallbackComponent::OnOdinPeerJoined_Implementation(UOdinRoom* Room, const FOdinPeerJoined& Data)
{
}

void UOdinRoomCallbackComponent::OnOdinRoomStatusChanged_Implementation(UOdinRoom* Room, const FOdinRoomStatusChanged& Data)
{
}

void UOdinRoomCallbackComponent::OnOdinRoomClosed_Implementation(UOdinRoom* Room, const FOdinRoomStatusChanged& Data)
{
}
