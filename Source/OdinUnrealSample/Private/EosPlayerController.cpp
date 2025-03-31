// Fill out your copyright notice in the Description page of Project Settings.


#include "EosPlayerController.h"
#include "Misc/ConfigCacheIni.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Containers/Array.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/ArchiveLoadCompressedProxy.h"

void AEosPlayerController::Login()
{
	/*
	Tutorial 2: This function will access the EOS OSS via the OSS identity interface to log first into Epic Account Services, and then into Epic Game Services.
	It will bind a delegate to handle the callback event once login call succeeeds or fails.
	All functions that access the OSS will have this structure: 1-Get OSS interface, 2-Bind delegate for callback and 3-Call OSS interface function (which will call the correspongin EOS OSS function)
	*/
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface(); // This is the generic OSS interface that will access the EOS OSS.

	// If you're logged in, don't try to login again.
	// This can happen if your player travels to a dedicated server or different maps as BeginPlay() will be called each time.

	FUniqueNetIdPtr NetId = Identity->GetUniquePlayerId(0);

	if (NetId != nullptr && Identity->GetLoginStatus(0) == ELoginStatus::LoggedIn)
	{
		UE_LOG(LogTemp, Log, TEXT("Already logged in. Finding sessions."));
		FindSessions();
		return;
	}

	/* This binds a delegate so we can run our function when the callback completes. 0 represents the player number.
	You should parametrize this Login function and pass the parameter here for splitscreen.
	*/
	LoginDelegateHandle =
		Identity->AddOnLoginCompleteDelegate_Handle(
			0,
			FOnLoginCompleteDelegate::CreateUObject(
				this,
				&ThisClass::HandleLoginCompleted));

	// Grab command line parameters. If empty call hardcoded login function - Hardcoded login function useful for Play In Editor. 
	FString AuthType;
	FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), AuthType);

	if (!AuthType.IsEmpty()) //If parameter is NOT empty we can autologin.
	{
		/*
		In most situations you will want to automatically log a player in using the parameters passed via CLI.
		For example, using the exchange code for the Epic Games Store.
		*/
		UE_LOG(LogTemp, Log, TEXT("Logging into EOS...")); // Log to the UE logs that we are trying to log in. 

		if (!Identity->AutoLogin(0))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to login... ")); // Log to the UE logs that we are trying to log in.
			// Clear our handle and reset the delegate.
			Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
			LoginDelegateHandle.Reset();
		}
	}
	else
	{
		/*
		Fallback if the CLI parameters are empty.Useful for PIE.
		The type here could be developer if using the DevAuthTool, ExchangeCode if the game is launched via the Epic Games Launcher, etc...
		*/
		FOnlineAccountCredentials Credentials("persistentauth", "", "");

		UE_LOG(LogTemp, Log, TEXT("Logging into EOS with persistentauth...")); // Log to the UE logs that we are trying to log in. 

		if (!Identity->Login(0, Credentials))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to login with persistentauth, trying accountportal... ")); // Log to the UE logs that we are trying to log in.
			FOnlineAccountCredentials Credentials2("accountportal", "", "");
			
			if (!Identity->Login(0, Credentials2))
			{
				// Clear our handle and reset the delegate.
				Identity->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
				LoginDelegateHandle.Reset();
			}
		}
	}
}

void AEosPlayerController::Logout()
{
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	auto Session = Subsystem->GetSessionInterface();

	DestroySessionDelegateHandle =
		Session->AddOnDestroySessionCompleteDelegate_Handle(
			FOnDestroySessionCompleteDelegate::CreateUObject(
				this,
				&ThisClass::HandleLogoutCompleted));

	Session->DestroySession(LobbyName);
}

void AEosPlayerController::HandleLogoutCompleted(FName SessionName, bool bWasSuccessful)
{
}


void AEosPlayerController::HandleLoginCompleted(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	/*
	Tutorial 2: This function handles the callback from logging in. You should not proceed with any EOS features until this function is called.
	This function will remove the delegate that was bound in the Login() function.
	*/
	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineIdentityPtr Identity = Subsystem->GetIdentityInterface();
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Login callback completed!"));
		UE_LOG(LogTemp, Log, TEXT("Searching for a session..."));
		// Maybe via button or player action? Maybe add parameters here
		FindSessions();
	}
	else //Login failed
	{
		// If your game is online only, you may want to return an errror to the user and return to a menu that uses a different GameMode/PlayerController.

		UE_LOG(LogTemp, Warning, TEXT("EOS login failed.")); //Print sign in failure in logs as a warning.
	}

	Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, LoginDelegateHandle);
	LoginDelegateHandle.Reset();
}

//Default class constructor included here for completeness
AEosPlayerController::AEosPlayerController()
{
	// Tutorial 2: Including constructor here for clarity. Nothing added in derived class for this tutorial.
}

void AEosPlayerController::BeginPlay()
{
	// Tutorial 2: On BeginPlay call our login function. This is only on the GameClient, not on the DedicatedServer. 
	Super::BeginPlay(); // Call parent class BeginPlay() function
	//Login(); //Call login function 

	GConfig->GetString(TEXT("OnlineSubsystem"), TEXT("DefaultPlatformService"), SelectedSubsystem, GEngineIni);
}

void AEosPlayerController::CreateLobby(FName KeyName, FString KeyValue)
{
	// Tutorial 7: Create lobby - this code is similar to creating session, notice that bIsDedicated is false, bUseLobbiesIfAvailable and UseLobbiesVoiceChatIfAvailable is true

	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	CreateLobbyDelegateHandle =
		Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(
			this,
			&ThisClass::HandleCreateLobbyCompleted));

	TSharedRef<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
	SessionSettings->NumPublicConnections = 25; //We will test our sessions with 2 players to keep things simple
	SessionSettings->bShouldAdvertise = true; //This creates a public match and will be searchable.
	SessionSettings->bUsesPresence = false;   //No presence on dedicated server. This requires a local user.
	SessionSettings->bAllowJoinViaPresence = false;
	SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
	SessionSettings->bAllowInvites = false;    //Allow inviting players into session. This requires presence and a local user. 
	SessionSettings->bAllowJoinInProgress = true; //Once the session is started, no one can join.
	SessionSettings->bIsDedicated = false; //Session created on dedicated server.
	SessionSettings->bUseLobbiesIfAvailable = true; //For P2P we will use a lobby instead of a session
	SessionSettings->bUseLobbiesVoiceChatIfAvailable = false; //We will also enable voice
	SessionSettings->bUsesStats = true; //Needed to keep track of player stats.
	SessionSettings->Settings.Add(KeyName, FOnlineSessionSetting((KeyValue), EOnlineDataAdvertisementType::ViaOnlineService));

	UE_LOG(LogTemp, Log, TEXT("Creating Lobby..."));

	if (!Session->CreateSession(0, LobbyName, *SessionSettings))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create Lobby!"));
	}
}

void AEosPlayerController::HandleCreateLobbyCompleted(FName EOSLobbyName, bool bWasSuccessful)
{
	// Tutorial 7: Callback function: This is called once our lobby is created

	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Lobby: %s Created!"), *EOSLobbyName.ToString());
		FString Map = "/Game/Maps/TopDownExampleMap?listen"; //Hardcoding map name here, should be passed by parameter
		FURL TravelURL;
		TravelURL.Map = Map;
		GetWorld()->ServerTravel(Map, true);
		GetWorld()->Listen(TravelURL);
		SetupNotifications(); // Setup our listeners for lobby notification events 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create lobby!"));
	}

	// Clear our handle and reset the delegate. 
	Session->ClearOnCreateSessionCompleteDelegate_Handle(CreateLobbyDelegateHandle);
	CreateLobbyDelegateHandle.Reset();
}

void AEosPlayerController::SetupNotifications()
{
	
}

void AEosPlayerController::FindSessions(FName SearchKey, FString SearchValue) //put default value for example 
{
	// Tutorial 4: This function will find our EOS Session that was created by our DedicatedServer. 

	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	TSharedRef<FOnlineSessionSearch> Search = MakeShared<FOnlineSessionSearch>();

	// Remove the default search parameters that FOnlineSessionSearch sets up.
	Search->QuerySettings.SearchParams.Empty();

	Search->QuerySettings.Set(SearchKey, SearchValue, EOnlineComparisonOp::Equals); // Seach using our Key/Value pair
	Search->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	FindSessionsDelegateHandle =
		Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(
			this,
			&ThisClass::HandleFindSessionsCompleted,
			Search));

	UE_LOG(LogTemp, Log, TEXT("Finding session."));

	if (!Session->FindSessions(0, Search))
	{
		UE_LOG(LogTemp, Warning, TEXT("Find session failed"));
	}
}

void AEosPlayerController::HandleFindSessionsCompleted(bool bWasSuccessful, TSharedRef<FOnlineSessionSearch> Search)
{
	// Tutorial 4: This function is triggered via the callback we set in FindSession once the session is found (or there is a failure)

	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Found session."));

		if (Search->SearchResults.Num() > 0)
		{
			for (auto SessionInSearchResult : Search->SearchResults)
			{
				// Typically you want to check if the session is valid before joining. There is a bug in the EOS OSS where IsValid() returns false when the session is created on a DS. 
				// Instead of customizing the engine for this tutorial, we're simply not checking if the session is valid. The code below should go in this if statement once the bug is fixed. 
				/*
				if (SessionInSearchResult.IsValid())
				{


				}
				*/

				//Ensure the connection string is resolvable and store the info in ConnectInfo and in SessionToJoin
				if (Session->GetResolvedConnectString(SessionInSearchResult, NAME_GamePort, ConnectString))
				{
					SessionToJoin = &SessionInSearchResult;
				}

				// For the tutorial we will join the first session found automatically. Usually you would loop through all the sessions and determine which one is best to join. 
				break;
			}
			JoinSession();
		}
		else
		{
			CreateLobby();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Sessions failed.")); //print warning in logs of failure
	}

	Session->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsDelegateHandle);
	FindSessionsDelegateHandle.Reset();
}

void AEosPlayerController::JoinSession()
{
	// Tutorial 4: Join the session. 

	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	JoinSessionDelegateHandle =
		Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(
			this,
			&ThisClass::HandleJoinSessionCompleted));

	UE_LOG(LogTemp, Log, TEXT("Joining session."));
	if (!Session->JoinSession(0, LobbyName, *SessionToJoin))
	{
		UE_LOG(LogTemp, Warning, TEXT("Join session failed"));
	}
}

void AEosPlayerController::HandleJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	// Tutorial 4: This function is triggered via the callback we set in JoinSession once the session is joined (or there is a failure)

	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Log, TEXT("Joined lobby with name %s"), *(SessionName.ToString()));
		UE_LOG(LogTemp, Warning, TEXT("Open Level: %s"), *ConnectString);
		ClientTravel(ConnectString, TRAVEL_Absolute);
		SetupNotifications(); // Setup our listeners for lobby event notifications
	}
	Session->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionDelegateHandle);
	JoinSessionDelegateHandle.Reset();
}