// Fill out your copyright notice in the Description page of Project Settings.


#include "CopyToClipboardFunctionLibrary.h"
#include "HAL/PlatformApplicationMisc.h"

void UCopyToClipboardFunctionLibrary::CopyToClipboard(FString Text)
{
	FPlatformApplicationMisc::ClipboardCopy(*Text);
}
