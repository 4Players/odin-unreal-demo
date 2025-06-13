#pragma once

#include "Interfaces/OnlineIdentityInterface.h"

class FOnlineIdentityOdinFleet : public IOnlineIdentity
{
public:
	virtual ELoginStatus::Type GetLoginStatus(int32 LocalUserNum) const override
	{
		return ELoginStatus::LoggedIn;
	}

	virtual FUniqueNetIdPtr GetUniquePlayerId(int32) const override { return nullptr; }
	virtual FUniqueNetIdPtr CreateUniquePlayerId(const FString&) override { return nullptr; }
	virtual FString GetPlayerNickname(int32) const override { return FString(); }
	virtual FString GetAuthToken(int32) const override { return FString(); }
	virtual FPlatformUserId GetPlatformUserIdFromUniqueNetId(const FUniqueNetId&) const override { return PLATFORMUSERID_NONE; }
	virtual bool Login(int32, const FOnlineAccountCredentials&) override { return true; }
	virtual bool Logout(int32) override { return true; }
	virtual bool AutoLogin(int32) override { return true; }

	// Inherited via IOnlineIdentity
	TSharedPtr<FUserOnlineAccount> GetUserAccount(const FUniqueNetId& UserId) const override;
	TArray<TSharedPtr<FUserOnlineAccount>> GetAllUserAccounts() const override;
	FUniqueNetIdPtr CreateUniquePlayerId(uint8* Bytes, int32 Size) override;
	ELoginStatus::Type GetLoginStatus(const FUniqueNetId& UserId) const override;
	FString GetPlayerNickname(const FUniqueNetId& UserId) const override;
	void RevokeAuthToken(const FUniqueNetId& LocalUserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate) override;
	void GetUserPrivilege(const FUniqueNetId& LocalUserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate, EShowPrivilegeResolveUI ShowResolveUI) override;
	FString GetAuthType() const override;
};
