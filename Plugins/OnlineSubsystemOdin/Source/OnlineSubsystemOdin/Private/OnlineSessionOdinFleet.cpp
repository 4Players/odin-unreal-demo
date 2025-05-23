#include "OnlineSessionOdinFleet.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInfoOdinFleet.h"

bool FOnlineSessionOdinFleet::FindSessions(int32 LocalUserNum, const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	// Beispielhafte URL
	auto Request = FHttpModule::Get().CreateRequest();
	Request->SetURL("https://yourbackend.example.com/getserver");
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindLambda([SearchSettings](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		if (bWasSuccessful && Response->GetResponseCode() == 200)
		{
			// IP Adresse parsen (hier stark vereinfacht)
			FString IP = Response->GetContentAsString();
			FOnlineSessionSearchResult Result;
			Result.Session.SessionInfo = MakeShared<FOnlineSessionInfoOdinFleet>();
			SearchSettings->SearchResults.Add(Result);
			SearchSettings->SearchState = EOnlineAsyncTaskState::Done;
		}
		else
		{
			SearchSettings->SearchState = EOnlineAsyncTaskState::Failed;
		}
	});
	Request->ProcessRequest();

	return true;
}

bool FOnlineSessionOdinFleet::JoinSession(int32 LocalUserNum, FName SessionName, const FOnlineSessionSearchResult& DesiredSession)
{
	return true; // nichts tun, da Verbindung direkt erfolgt
}

FNamedOnlineSession* FOnlineSessionOdinFleet::AddNamedSession(FName SessionName, const FOnlineSessionSettings& SessionSettings)
{
	return nullptr;
}

FNamedOnlineSession* FOnlineSessionOdinFleet::AddNamedSession(FName SessionName, const FOnlineSession& Session)
{
	return nullptr;
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
	return false;
}

bool FOnlineSessionOdinFleet::FindSessionById(const FUniqueNetId& SearchingUserId, const FUniqueNetId& SessionId, const FUniqueNetId& FriendId, const FOnSingleSessionResultCompleteDelegate& CompletionDelegate)
{
	return false;
}

bool FOnlineSessionOdinFleet::JoinSession(const FUniqueNetId& LocalUserId, FName SessionName, const FOnlineSessionSearchResult& DesiredSession)
{
	return false;
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
	return false;
}

bool FOnlineSessionOdinFleet::GetResolvedConnectString(const FOnlineSessionSearchResult& SearchResult, FName PortType, FString& ConnectInfo)
{
	return false;
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
