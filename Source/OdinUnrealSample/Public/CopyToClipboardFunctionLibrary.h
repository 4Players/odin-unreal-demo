// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CopyToClipboardFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ODINUNREALSAMPLE_API UCopyToClipboardFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Platform|Clipboard")
	static void CopyToClipboard(FString Text);

	UFUNCTION(BlueprintCallable, Category = "Platform|SaveFile")
	static void SaveBackendConfigToJson(FString Text);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Platform|SaveFile")
	static FString LoadBackendConfigFromJson();
};
