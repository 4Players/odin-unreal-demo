// Fill out your copyright notice in the Description page of Project Settings.


#include "CopyToClipboardFunctionLibrary.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

void UCopyToClipboardFunctionLibrary::CopyToClipboard(FString Text)
{
	FPlatformApplicationMisc::ClipboardCopy(*Text);
}

void UCopyToClipboardFunctionLibrary::SaveBackendConfigToJson(FString Text)
{
	FString JsonPathRel = TEXT("OdinFleetConfig.json");


	FString JsonPathAbs = FPaths::Combine(FPaths::ProjectSavedDir(), JsonPathRel);

	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

	JsonObject->SetStringField(TEXT("Url"), Text);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to serialize JSON"));
		return;
	}

	if (FFileHelper::SaveStringToFile(OutputString, *JsonPathAbs))
	{
		UE_LOG(LogTemp, Log, TEXT("JSON written to %s"), *JsonPathAbs);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to write JSON to file"));
	}
}

FString UCopyToClipboardFunctionLibrary::LoadBackendConfigFromJson()
{
	FString JsonPathRel = TEXT("OdinFleetConfig.json");


	FString JsonPathAbs = FPaths::Combine(FPaths::ProjectSavedDir(), JsonPathRel);

	FString JsonString;
	if (FFileHelper::LoadFileToString(JsonString, *JsonPathAbs))
	{
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		TSharedPtr<FJsonObject> JsonObject;
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			if (!JsonObject->HasField(TEXT("Url"))) return "";

			return JsonObject->GetStringField("Url");
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON from %s"), *JsonPathAbs);
			return "";
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file from %s"), *JsonPathAbs);
		return "";
	}
}
