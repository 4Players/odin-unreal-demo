#include "OnlineIdentityOdinFleet.h"

TSharedPtr<FUserOnlineAccount> FOnlineIdentityOdinFleet::GetUserAccount(const FUniqueNetId& UserId) const
{
	return TSharedPtr<FUserOnlineAccount>();
}

TArray<TSharedPtr<FUserOnlineAccount>> FOnlineIdentityOdinFleet::GetAllUserAccounts() const
{
	return TArray<TSharedPtr<FUserOnlineAccount>>();
}

FUniqueNetIdPtr FOnlineIdentityOdinFleet::CreateUniquePlayerId(uint8* Bytes, int32 Size)
{
	return FUniqueNetIdPtr();
}

ELoginStatus::Type FOnlineIdentityOdinFleet::GetLoginStatus(const FUniqueNetId& UserId) const
{
	return ELoginStatus::Type();
}

FString FOnlineIdentityOdinFleet::GetPlayerNickname(const FUniqueNetId& UserId) const
{
	return FString();
}

void FOnlineIdentityOdinFleet::RevokeAuthToken(const FUniqueNetId& LocalUserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate)
{
}

void FOnlineIdentityOdinFleet::GetUserPrivilege(const FUniqueNetId& LocalUserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate, EShowPrivilegeResolveUI ShowResolveUI)
{
}

FString FOnlineIdentityOdinFleet::GetAuthType() const
{
	return FString();
}
