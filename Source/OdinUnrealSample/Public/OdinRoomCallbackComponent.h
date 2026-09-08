// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OdinNative/OdinNativeRpc.h"
#include "TimerManager.h"
#include "OdinRoomCallbackComponent.generated.h"

class UOdinDecoder;
class UOdinRoom;

UCLASS(ClassGroup = (Custom), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class ODINUNREALSAMPLE_API UOdinRoomCallbackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOdinRoomCallbackComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Odin|Room")
	void BindRoomCallbacks(UOdinRoom* Room);

	UFUNCTION(BlueprintCallable, Category = "Odin|Room")
	void UnbindRoomCallbacks(UOdinRoom* Room);

	UFUNCTION(BlueprintCallable, Category = "Odin|Room")
	void UnbindAllRoomCallbacks();

	UFUNCTION(BlueprintCallable, Category = "Odin|Cleanup")
	int32 CleanupPeerAudio(UOdinRoom* Room, int64 PeerId, bool bDestroySynthComponents = true, bool bFreeDecoders = true);

	UFUNCTION(BlueprintCallable, Category = "Odin|Cleanup")
	int32 CleanupDecoderAudio(UOdinDecoder* Decoder, bool bDestroySynthComponents = true, bool bFreeDecoder = true);

	// Stops callback-driven cleanup before the game instance tears down all ODIN objects in this world.
	void PrepareForWorldTeardown();

protected:
	UFUNCTION()
	void HandleRoomPeerLeft(UOdinRoom* Room, FOdinPeerLeft Data);

	UFUNCTION()
	void HandleRoomStatusChanged(UOdinRoom* Room, FOdinRoomStatusChanged Data);

	UFUNCTION(BlueprintNativeEvent, Category = "Odin|Room")
	void OnOdinPeerLeft(UOdinRoom* Room, const FOdinPeerLeft& Data);
	virtual void OnOdinPeerLeft_Implementation(UOdinRoom* Room, const FOdinPeerLeft& Data);

	// Kept so existing Blueprint nodes remain loadable; peer joins should bind to OnRoomPeerJoinedBP directly.
	UFUNCTION(BlueprintNativeEvent, Category = "Odin|Room", meta = (DeprecatedFunction, DeprecationMessage = "Bind to the room's OnRoomPeerJoinedBP event directly."))
	void OnOdinPeerJoined(UOdinRoom* Room, const FOdinPeerJoined& Data);
	virtual void OnOdinPeerJoined_Implementation(UOdinRoom* Room, const FOdinPeerJoined& Data);

	UFUNCTION(BlueprintNativeEvent, Category = "Odin|Room")
	void OnOdinRoomStatusChanged(UOdinRoom* Room, const FOdinRoomStatusChanged& Data);
	virtual void OnOdinRoomStatusChanged_Implementation(UOdinRoom* Room, const FOdinRoomStatusChanged& Data);

	UFUNCTION(BlueprintNativeEvent, Category = "Odin|Room")
	void OnOdinRoomClosed(UOdinRoom* Room, const FOdinRoomStatusChanged& Data);
	virtual void OnOdinRoomClosed_Implementation(UOdinRoom* Room, const FOdinRoomStatusChanged& Data);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Odin|Room")
	TArray<TObjectPtr<UOdinRoom>> AutoBindRooms;

private:
	void FlushPendingAudioCleanup();

	UPROPERTY(Transient)
	TArray<TObjectPtr<UOdinRoom>> BoundRooms;

	UPROPERTY(Transient)
	TArray<TObjectPtr<class UOdinSynthComponent>> PendingSynthCleanup;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UOdinDecoder>> PendingDecoderCleanup;

	UPROPERTY(EditAnywhere, Category = "Odin|Cleanup", meta = (ClampMin = "0.01", Units = "s"))
	float AudioCleanupGracePeriod = 0.1f;

	FTimerHandle AudioCleanupTimer;
	bool bOdinTeardownStarted = false;
};
