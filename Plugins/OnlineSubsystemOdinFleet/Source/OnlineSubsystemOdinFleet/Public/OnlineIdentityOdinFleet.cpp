#include "OnlineIdentityOdinFleet.h"
#include "OnlineError.h"

TSharedPtr<FUserOnlineAccount> FOnlineIdentityOdinFleet::GetUserAccount(const FUniqueNetId& UserId) const
{
	const FUniqueNetIdString* IdStr = static_cast<const FUniqueNetIdString*>(&UserId);
	if (IdStr && UserAccounts.Contains(*IdStr))
		return UserAccounts[*IdStr];
	return nullptr;
}

TArray<TSharedPtr<FUserOnlineAccount>> FOnlineIdentityOdinFleet::GetAllUserAccounts() const
{
	TArray<TSharedPtr<FUserOnlineAccount>> Accounts;
	UserAccounts.GenerateValueArray(Accounts);
	return Accounts;
}

FUniqueNetIdPtr FOnlineIdentityOdinFleet::CreateUniquePlayerId(uint8* Bytes, int32 Size)
{
	return MakeShared<FUniqueNetIdOdinFleet>(FString::FromHexBlob(Bytes, Size));
}

ELoginStatus::Type FOnlineIdentityOdinFleet::GetLoginStatus(const FUniqueNetId& UserId) const
{
	const FUniqueNetIdString* IdStr = static_cast<const FUniqueNetIdString*>(&UserId);
	return (IdStr && UserAccounts.Contains(*IdStr)) ? ELoginStatus::LoggedIn : ELoginStatus::NotLoggedIn;
}

FString FOnlineIdentityOdinFleet::GetPlayerNickname(const FUniqueNetId& UserId) const
{
	return UserId.IsValid() ? UserId.ToString() : TEXT("Unknown");
}

void FOnlineIdentityOdinFleet::RevokeAuthToken(const FUniqueNetId& LocalUserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate)
{
	Delegate.ExecuteIfBound(LocalUserId, FOnlineError(false));
}

void FOnlineIdentityOdinFleet::GetUserPrivilege(const FUniqueNetId& LocalUserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate, EShowPrivilegeResolveUI ShowResolveUI)
{
	
}

FString FOnlineIdentityOdinFleet::GetAuthType() const
{
	return TEXT("OdinFleet");
}
