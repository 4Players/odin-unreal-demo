// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionClient.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
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

	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
	{
		if (Result == EOnJoinSessionCompleteResult::Success)
		{
			// Handle successful join session logic here
			UE_LOG(LogTemp, Log, TEXT("Successfully joined session: %s"), *SessionName.ToString());

			UWorld* World = GetWorld();
			if (!World) return;

			APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);

			IOnlineSessionPtr Session = IOnlineSubsystem::Get()->GetSessionInterface();
			if (!Session.IsValid()) return;

			FString ConnectString;

			Session->GetResolvedConnectString(SessionName, ConnectString, NAME_GamePort);

			if (PC && PC->PlayerState)
			{
				PC->ClientTravel(ConnectString, TRAVEL_Absolute);
			}
		}
		else
		{
			// Handle failure to join session
			UE_LOG(LogTemp, Warning, TEXT("Failed to join session: %s"), *SessionName.ToString());
		}
	}
};
