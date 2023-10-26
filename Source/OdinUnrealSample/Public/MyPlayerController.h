// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ODINUNREALSAMPLE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable)
		virtual void GetAudioListenerPositionBP
		(
			FVector& OutLocation,
			FVector& OutFrontDir,
			FVector& OutRightDir
		) const;
};
