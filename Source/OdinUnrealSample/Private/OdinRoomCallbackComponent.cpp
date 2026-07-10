// Fill out your copyright notice in the Description page of Project Settings.

#include "OdinRoomCallbackComponent.h"

#include "Components/AudioComponent.h"
#include "OdinAudio/OdinDecoder.h"
#include "OdinAudio/OdinSynthComponent.h"
#include "OdinFunctionLibrary.h"
#include "OdinRoom.h"

UOdinRoomCallbackComponent::UOdinRoomCallbackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UOdinRoomCallbackComponent::BeginPlay()
{
	Super::BeginPlay();

	for (UOdinRoom* Room : AutoBindRooms)
	{
		BindRoomCallbacks(Room);
	}
}

void UOdinRoomCallbackComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnbindAllRoomCallbacks();
	Super::EndPlay(EndPlayReason);
}

void UOdinRoomCallbackComponent::BindRoomCallbacks(UOdinRoom* Room)
{
	if (!IsValid(Room) || BoundRooms.Contains(Room))
	{
		return;
	}

	Room->OnRoomPeerLeftBP.AddDynamic(this, &ThisClass::HandleRoomPeerLeft);
	Room->OnRoomStatusChangedBP.AddDynamic(this, &ThisClass::HandleRoomStatusChanged);
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
	if (!IsValid(Room))
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
	if (!IsValid(Decoder))
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
			SynthComponent->DestroyComponent();
			++CleanedObjects;
		}
	}

	if (bFreeDecoder && UOdinDecoder::FreeDecoder(Decoder))
	{
		++CleanedObjects;
	}

	return CleanedObjects;
}

void UOdinRoomCallbackComponent::HandleRoomPeerLeft(UOdinRoom* Room, FOdinPeerLeft Data)
{
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
	CleanupPeerAudio(Room, Data.peer_id);
}

void UOdinRoomCallbackComponent::OnOdinRoomStatusChanged_Implementation(UOdinRoom* Room, const FOdinRoomStatusChanged& Data)
{
}

void UOdinRoomCallbackComponent::OnOdinRoomClosed_Implementation(UOdinRoom* Room, const FOdinRoomStatusChanged& Data)
{
}
