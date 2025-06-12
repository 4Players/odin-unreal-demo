// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EosPlayerController.generated.h"

class FOnlineSessionSearch;
class FOnlineSessionSearchResult;

/**
* Child class of APlayerController to hold EOS OSS code.
*/
UCLASS()
class AEosPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	// Class constructor. We won't use this in this tutorial. 
	AEosPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SelectedSubsystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SessionId;

	// Get a string copy of the current session ID
	UFUNCTION(BlueprintPure, Category = "Online|EosSessions|SessionInfo")
	void GetCurrentSessionID_AsString(FString& ResultSessionID);

protected:
	// Function called when play begins
	virtual void BeginPlay();

	UFUNCTION(BlueprintCallable)
	//Function to sign into EOS Game Services
	void Login();

	UFUNCTION(BlueprintCallable)
	//Function to sign into EOS Game Services
	void GetOnlineSubsystem();

	UFUNCTION(BlueprintCallable)
	//Function to sign into EOS Game Services
	void Logout();

	void HandleLogoutCompleted(FName SessionName, bool bWasSuccessful);

	FDelegateHandle LogoutDelegateHandle;

	//Callback function. This function is ran when signing into EOS Game Services completes. 
	void HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	bool bTriedAuthPortal;

	//Delegate to bind callback event for login. 
	FDelegateHandle LoginDelegateHandle;

	// Hardcoded name for the lobby.
	FName LobbyName = "LobbyName";
	// Function to create an EOS session. 
	void CreateLobby(FName KeyName = "KeyName", FString KeyValue = "KeyValue");

	// Callback function. This function will run when creating the session compeletes. 
	void HandleCreateLobbyCompleted(FName LobbyName, bool bWasSuccessful);

	// Delegate to bind callback event for session creation.
	FDelegateHandle CreateLobbyDelegateHandle;

	// Function used to setup our listeners to lobby notification events - example on participant change only.
	void SetupNotifications();

	// Callback function. This function will run when a lobby participant joins / leaves.
	void HandleParticipantChanged(FName EOSLobbyName, const FUniqueNetId& NetId, bool bJoined);

	// Function to find EOS sessions. Hardcoded attribute key/value pair to keep things simple
	void FindSessions(FName SearchKey = "KeyName", FString SearchValue = "KeyValue");

	// Callback function. This function will run when the session is found.
	void HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search);

	//Delegate to bind callback event for when sessions are found.
	FDelegateHandle FindSessionsDelegateHandle;

	// This is the connection string for the client to connect to the dedicated server.
	FString ConnectString;

	// This is used to store the session to join information from the search. You could pass it as a paramter to JoinSession() instead. 
	FOnlineSessionSearchResult* SessionToJoin;

	// Function to join the EOS session. 
	void JoinSession();

	// Callback function. This function will run when the session is joined. 
	void HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// Delegate to bind callback event for join session.
	FDelegateHandle JoinSessionDelegateHandle;

	FDelegateHandle DestroySessionDelegateHandle;
};
