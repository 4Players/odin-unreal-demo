#include "OnlineSessionOdinFleet.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSessionInfoOdinFleet.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Json.h"

bool FOnlineSessionOdinFleet::FindSessions(int32 LocalUserNum, const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	TSharedRef<FOnlineSessionOdinFleet> SessionRef = SharedThis(this);

	LoadConfiguration();

	auto Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindLambda([SessionRef, SearchSettings](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (!bWasSuccessful || !Response.IsValid() || Response->GetResponseCode() != 200)
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to find session from Azure Function."));
				SessionRef->TriggerOnFindSessionsCompleteDelegates(false);
				return;
			}

			FString Body = Response->GetContentAsString();
			TSharedPtr<FJsonObject> Json;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Body);

			if (!FJsonSerializer::Deserialize(Reader, Json) || !Json.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid JSON from Azure Function."));
				SessionRef->TriggerOnFindSessionsCompleteDelegates(false);
				return;
			}

			FString Ip = Json->GetStringField("ip");
			int32 Port = Json->GetIntegerField("port");

			// Create session result
			FOnlineSessionSearchResult Result;
			FOnlineSession Session;
			Session.SessionInfo = MakeShared<FOnlineSessionInfoOdinFleet>(Ip, Port);
			Session.SessionSettings.NumPublicConnections = 2;
			Session.SessionSettings.bIsLANMatch = false;
			Session.SessionSettings.bIsDedicated = true;
			Session.OwningUserName = TEXT("OdinServer");

			Result.Session = Session;
			SearchSettings->SearchResults.Add(Result);

			SessionRef->TriggerOnFindSessionsCompleteDelegates(true);
		});
	Request->ProcessRequest();

	return true;
}

bool FOnlineSessionOdinFleet::JoinSession(int32 LocalUserNum, FName SessionName, const FOnlineSessionSearchResult& DesiredSession)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("JoinSession failed: no subsystem"));
		return false;
	}

	const FOnlineSessionInfoOdinFleet* OdinSessionInfo = static_cast<const FOnlineSessionInfoOdinFleet*>(DesiredSession.Session.SessionInfo.Get());
	if (!OdinSessionInfo || !OdinSessionInfo->IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("JoinSession failed: Invalid session info"));
		return false;
	}

	FNamedOnlineSession* Session = GetNamedSession(SessionName);
	if (!Session)
	{
		Session = AddNamedSession(SessionName, DesiredSession.Session);
	}
	else
	{
		Session->SessionSettings = DesiredSession.Session.SessionSettings;
		Session->SessionState = EOnlineSessionState::Pending;
	}

	Session->HostingPlayerNum = LocalUserNum;
	Session->SessionInfo = DesiredSession.Session.SessionInfo;

	

	

	//PC->ClientTravel(TravelURL, TRAVEL_Absolute);

	// Notify game that join succeeded
	TriggerOnJoinSessionCompleteDelegates(SessionName, EOnJoinSessionCompleteResult::Success);
	return true;
}

FNamedOnlineSession* FOnlineSessionOdinFleet::AddNamedSession(FName SessionName, const FOnlineSessionSettings& SessionSettings)
{
	// Gibt es schon eine Session mit diesem Namen?
	if (GetNamedSession(SessionName) != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session with name '%s' already exists."), *SessionName.ToString());
		return nullptr;
	}

	// Neue Session anlegen
	TSharedPtr<FNamedOnlineSession> NewSession = MakeShared<FNamedOnlineSession>(SessionName, SessionSettings);

	// In Session-Liste eintragen
	Sessions.Add(SessionName, NewSession);

	UE_LOG(LogTemp, Log, TEXT("Added new session '%s'."), *SessionName.ToString());

	return NewSession.Get();
}

FNamedOnlineSession* FOnlineSessionOdinFleet::AddNamedSession(FName SessionName, const FOnlineSession& Session)
{
	return AddNamedSession(SessionName, Session.SessionSettings);
}

FUniqueNetIdPtr FOnlineSessionOdinFleet::CreateSessionIdFromString(const FString& SessionIdStr)
{
	return FUniqueNetIdPtr();
}

bool FOnlineSessionOdinFleet::HasPresenceSession()
{
	return false;
}

bool FOnlineSessionOdinFleet::CreateSession(const FUniqueNetId& HostingPlayerId, FName SessionName, const FOnlineSessionSettings& NewSessionSettings)
{
	return false;
}

bool FOnlineSessionOdinFleet::UpdateSession(FName SessionName, FOnlineSessionSettings& UpdatedSessionSettings, bool bShouldRefreshOnlineData)
{
	return false;
}

bool FOnlineSessionOdinFleet::StartMatchmaking(const TArray<FUniqueNetIdRef>& LocalPlayers, FName SessionName, const FOnlineSessionSettings& NewSessionSettings, TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	return false;
}

bool FOnlineSessionOdinFleet::CancelMatchmaking(const FUniqueNetId& SearchingPlayerId, FName SessionName)
{
	return false;
}

bool FOnlineSessionOdinFleet::FindSessions(const FUniqueNetId& SearchingPlayerId, const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineIdentityPtr IdentityInterface = Subsystem ? Subsystem->GetIdentityInterface() : nullptr;

	if (!IdentityInterface) return false;

	return FindSessions(0, SearchSettings);
}

bool FOnlineSessionOdinFleet::FindSessionById(const FUniqueNetId& SearchingUserId, const FUniqueNetId& SessionId, const FUniqueNetId& FriendId, const FOnSingleSessionResultCompleteDelegate& CompletionDelegate)
{
	return false;
}

bool FOnlineSessionOdinFleet::JoinSession(const FUniqueNetId& LocalUserId, FName SessionName, const FOnlineSessionSearchResult& DesiredSession)
{
	return JoinSession(0, SessionName, DesiredSession);
}

bool FOnlineSessionOdinFleet::FindFriendSession(int32 LocalUserNum, const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionOdinFleet::FindFriendSession(const FUniqueNetId& LocalUserId, const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionOdinFleet::FindFriendSession(const FUniqueNetId& LocalUserId, const TArray<FUniqueNetIdRef>& FriendList)
{
	return false;
}

bool FOnlineSessionOdinFleet::SendSessionInviteToFriend(const FUniqueNetId& LocalUserId, FName SessionName, const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionOdinFleet::SendSessionInviteToFriends(const FUniqueNetId& LocalUserId, FName SessionName, const TArray<FUniqueNetIdRef>& Friends)
{
	return false;
}

bool FOnlineSessionOdinFleet::GetResolvedConnectString(FName SessionName, FString& ConnectInfo, FName PortType)
{
	FNamedOnlineSession* Session = GetNamedSession(SessionName);
	if (!Session) return false;

	TSharedPtr<FOnlineSessionInfo> Info = Session->SessionInfo;
	TSharedPtr<FOnlineSessionInfoOdinFleet> OdinInfo = StaticCastSharedPtr<FOnlineSessionInfoOdinFleet>(Info);

	if (!OdinInfo.IsValid() || !OdinInfo->HostAddr->IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid session info"));
		return false;
	}

	FString Address = OdinInfo->HostAddr->ToString(false); // IP only
	int32 Port = OdinInfo->HostAddr->GetPort();

	ConnectInfo = FString::Printf(TEXT("%s:%d"), *Address, Port);

	return true;
}

bool FOnlineSessionOdinFleet::GetResolvedConnectString(const FOnlineSessionSearchResult& SearchResult, FName PortType, FString& ConnectInfo)
{
	TSharedPtr<FOnlineSessionInfo> Info = SearchResult.Session.SessionInfo;
	TSharedPtr<FOnlineSessionInfoOdinFleet> OdinInfo = StaticCastSharedPtr<FOnlineSessionInfoOdinFleet>(Info);

	if (!OdinInfo.IsValid() || !OdinInfo->HostAddr->IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid session info"));
		return false;
	}

	FString Address = OdinInfo->HostAddr->ToString(false); // IP only
	int32 Port = OdinInfo->HostAddr->GetPort();

	ConnectInfo = FString::Printf(TEXT("%s:%d"), *Address, Port);

	return true;
}

FOnlineSessionSettings* FOnlineSessionOdinFleet::GetSessionSettings(FName SessionName)
{
	return nullptr;
}

bool FOnlineSessionOdinFleet::RegisterPlayer(FName SessionName, const FUniqueNetId& PlayerId, bool bWasInvited)
{
	return false;
}

bool FOnlineSessionOdinFleet::RegisterPlayers(FName SessionName, const TArray<FUniqueNetIdRef>& Players, bool bWasInvited)
{
	return false;
}

bool FOnlineSessionOdinFleet::UnregisterPlayer(FName SessionName, const FUniqueNetId& PlayerId)
{
	return false;
}

bool FOnlineSessionOdinFleet::UnregisterPlayers(FName SessionName, const TArray<FUniqueNetIdRef>& Players)
{
	return false;
}

void FOnlineSessionOdinFleet::RegisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName, const FOnRegisterLocalPlayerCompleteDelegate& Delegate)
{
}

void FOnlineSessionOdinFleet::UnregisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName, const FOnUnregisterLocalPlayerCompleteDelegate& Delegate)
{
}

bool FOnlineSessionOdinFleet::LoadConfiguration() 
{
	FString JsonPathRel = FPaths::Combine(TEXT("Data"), TEXT("OdinFleetConfig.json"));

#if WITH_EDITOR
	FString JsonPathAbs = FPaths::Combine(FPaths::ProjectDir(), JsonPathRel);
#else
	FString JsonPathAbs = FPaths::Combine(FPaths::LaunchDir(), JsonPathRel);
#endif

	FString JsonString;
	if (FFileHelper::LoadFileToString(JsonString, *JsonPathAbs))
	{
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
		TSharedPtr<FJsonObject> JsonObject;
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			Url = JsonObject->GetStringField("Url");
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON from %s"), *JsonPathAbs);
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file from %s"), *JsonPathAbs);
		return false;
	}
}
