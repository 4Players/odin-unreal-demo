// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

void AMyPlayerController::GetAudioListenerPositionBP(FVector& OutLocation, FVector& OutFrontDir, FVector& OutRightDir) const
{
	GetAudioListenerPosition(OutLocation, OutFrontDir, OutRightDir);
}
