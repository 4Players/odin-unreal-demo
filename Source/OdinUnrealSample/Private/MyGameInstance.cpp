// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"


void UMyGameInstance::Init()
{
    IOnlineIdentityPtr Identity = IOnlineSubsystem::Get()->GetIdentityInterface();
    if (Identity.IsValid())
    {
        Identity->AddOnLoginCompleteDelegate_Handle(0, FOnLoginCompleteDelegate::CreateUObject(this, &UMyGameInstance::OnLoginComplete));
    }
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