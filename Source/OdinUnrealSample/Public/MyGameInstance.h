// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionClient.h"
#include "TimerManager.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ODINUNREALSAMPLE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void Init() override;
	void Shutdown() override;
	void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;

	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void HandleWorldBeginTearDown(UWorld* World);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
	void CleanupOdinBeforeTravel(const TCHAR* Reason, UWorld* WorldToClean = nullptr);
	void ReleaseRetiredOdinObjects();

	UPROPERTY(Transient)
	TArray<TObjectPtr<class UOdinAudioCapture>> RetiredOdinCaptures;

	UPROPERTY(Transient)
	TArray<TObjectPtr<class UOdinEncoder>> RetiredOdinEncoders;

	FDelegateHandle WorldBeginTearDownHandle;
	FTimerHandle RetiredOdinReleaseTimer;
	bool bOdinCleanupTriggered = false;
};
