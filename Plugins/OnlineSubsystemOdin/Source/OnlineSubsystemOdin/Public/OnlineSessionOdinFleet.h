#pragma once

#include "Interfaces/OnlineSessionInterface.h"

class FOnlineSessionOdinFleet : public IOnlineSession
{
public:
	virtual bool CreateSession(int32, FName, const FOnlineSessionSettings&) override { return false; }
	virtual bool StartSession(FName) override { return false; }
	virtual bool EndSession(FName) override { return false; }
	virtual bool DestroySession(FName, const FOnDestroySessionCompleteDelegate&) override { return false; }
	virtual bool IsPlayerInSession(FName, const FUniqueNetId&) override { return false; }
	virtual bool CancelMatchmaking(int32, FName) override { return false; }
	virtual bool FindSessions(int32 LocalUserNum, const TSharedRef<FOnlineSessionSearch>& SearchSettings) override;
	virtual bool CancelFindSessions() override { return false; }
	virtual bool PingSearchResults(const FOnlineSessionSearchResult&) override { return false; }
	virtual bool JoinSession(int32 LocalUserNum, FName SessionName, const FOnlineSessionSearchResult& DesiredSession) override;
	virtual bool SendSessionInviteToFriend(int32, FName, const FUniqueNetId&) override { return false; }
	virtual bool SendSessionInviteToFriends(int32, FName, const TArray<TSharedRef<const FUniqueNetId>>&) override { return false; }
	virtual int32 GetNumSessions() override { return 0; }
	virtual void DumpSessionState() override {}
	virtual FNamedOnlineSession* GetNamedSession(FName) override { return nullptr; }
	virtual void RemoveNamedSession(FName) override {}
	virtual EOnlineSessionState::Type GetSessionState(FName) const override { return EOnlineSessionState::NoSession; }

	// Inherited via IOnlineSession
	FNamedOnlineSession* AddNamedSession(FName SessionName, const FOnlineSessionSettings& SessionSettings) override;
	FNamedOnlineSession* AddNamedSession(FName SessionName, const FOnlineSession& Session) override;
	FUniqueNetIdPtr CreateSessionIdFromString(const FString& SessionIdStr) override;
	bool HasPresenceSession() override;
	bool CreateSession(const FUniqueNetId& HostingPlayerId, FName SessionName, const FOnlineSessionSettings& NewSessionSettings) override;
	bool UpdateSession(FName SessionName, FOnlineSessionSettings& UpdatedSessionSettings, bool bShouldRefreshOnlineData) override;
	bool StartMatchmaking(const TArray<FUniqueNetIdRef>& LocalPlayers, FName SessionName, const FOnlineSessionSettings& NewSessionSettings, TSharedRef<FOnlineSessionSearch>& SearchSettings) override;
	bool CancelMatchmaking(const FUniqueNetId& SearchingPlayerId, FName SessionName) override;
	bool FindSessions(const FUniqueNetId& SearchingPlayerId, const TSharedRef<FOnlineSessionSearch>& SearchSettings) override;
	bool FindSessionById(const FUniqueNetId& SearchingUserId, const FUniqueNetId& SessionId, const FUniqueNetId& FriendId, const FOnSingleSessionResultCompleteDelegate& CompletionDelegate) override;
	bool JoinSession(const FUniqueNetId& LocalUserId, FName SessionName, const FOnlineSessionSearchResult& DesiredSession) override;
	bool FindFriendSession(int32 LocalUserNum, const FUniqueNetId& Friend) override;
	bool FindFriendSession(const FUniqueNetId& LocalUserId, const FUniqueNetId& Friend) override;
	bool FindFriendSession(const FUniqueNetId& LocalUserId, const TArray<FUniqueNetIdRef>& FriendList) override;
	bool SendSessionInviteToFriend(const FUniqueNetId& LocalUserId, FName SessionName, const FUniqueNetId& Friend) override;
	bool SendSessionInviteToFriends(const FUniqueNetId& LocalUserId, FName SessionName, const TArray<FUniqueNetIdRef>& Friends) override;
	bool GetResolvedConnectString(FName SessionName, FString& ConnectInfo, FName PortType) override;
	bool GetResolvedConnectString(const FOnlineSessionSearchResult& SearchResult, FName PortType, FString& ConnectInfo) override;
	FOnlineSessionSettings* GetSessionSettings(FName SessionName) override;
	bool RegisterPlayer(FName SessionName, const FUniqueNetId& PlayerId, bool bWasInvited) override;
	bool RegisterPlayers(FName SessionName, const TArray<FUniqueNetIdRef>& Players, bool bWasInvited) override;
	bool UnregisterPlayer(FName SessionName, const FUniqueNetId& PlayerId) override;
	bool UnregisterPlayers(FName SessionName, const TArray<FUniqueNetIdRef>& Players) override;
	void RegisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName, const FOnRegisterLocalPlayerCompleteDelegate& Delegate) override;
	void UnregisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName, const FOnUnregisterLocalPlayerCompleteDelegate& Delegate) override;
};