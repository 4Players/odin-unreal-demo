#pragma once

#include "Interfaces/OnlineIdentityInterface.h"

class FUniqueNetIdOdinFleet : public FUniqueNetIdString
{
public:
	explicit FUniqueNetIdOdinFleet(const FString& InIdStr)
		: IdStr(InIdStr)
	{
		IdBytes = TArray<uint8>((const uint8*)TCHAR_TO_ANSI(*IdStr), IdStr.Len());
	}

	virtual const uint8* GetBytes() const override { return IdBytes.GetData(); }
	virtual int32 GetSize() const override { return IdBytes.Num(); }
	virtual bool IsValid() const override { return !IdStr.IsEmpty(); }

	virtual FString ToString() const override { return IdStr; }
	virtual FString ToDebugString() const override { return FString::Printf(TEXT("OdinId:%s"), *IdStr); }

	virtual FName GetType() const override { return TEXT("OdinFleet"); }

protected:
	FString IdStr;
	TArray<uint8> IdBytes;
};

class FUserOnlineAccountOdinFleet : public FUserOnlineAccount
{
public:
	FUserOnlineAccountOdinFleet(const FString& InUserId, const FString& InToken)
		: UserId(MakeShared<FUniqueNetIdOdinFleet>(InUserId)), AccessToken(InToken) 
	{
	}

	FUserOnlineAccountOdinFleet(const FString& InUserId)
		: FUserOnlineAccountOdinFleet(InUserId, TEXT("None"))
	{
	}

	virtual TSharedRef<const FUniqueNetId> GetUserId() const override { return UserId.ToSharedRef(); }
	virtual FString GetRealName() const override { return TEXT("Odin User"); }
	virtual FString GetAccessToken() const override { return AccessToken; }

	virtual bool GetUserAttribute(const FString& AttrName, FString& OutAttr) const override { return false; }
	virtual bool SetUserAttribute(const FString& AttrName, const FString& AttrValue) override { return false; }
	FString GetDisplayName(const FString& Platform) const { return TEXT("Odin User"); }

	bool GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const { return false; }

private:
	TSharedPtr<FUniqueNetIdOdinFleet> UserId;
	FString AccessToken;
};

class FOnlineIdentityOdinFleet : public IOnlineIdentity
{
public:
	virtual ELoginStatus::Type GetLoginStatus(int32 LocalUserNum) const override
	{
		return UserIds.Contains(LocalUserNum) ? ELoginStatus::LoggedIn : ELoginStatus::NotLoggedIn;
	}

	virtual FUniqueNetIdPtr GetUniquePlayerId(int32 LocalUserNum) const override { return UserIds.FindRef(LocalUserNum); }
	virtual FUniqueNetIdPtr CreateUniquePlayerId(const FString&) override { return nullptr; }
	virtual FString GetPlayerNickname(int32 LocalUserNum) const override { return ""; }
	virtual FString GetAuthToken(int32) const override { return ""; }
	virtual FPlatformUserId GetPlatformUserIdFromUniqueNetId(const FUniqueNetId&) const override { return PLATFORMUSERID_NONE; }
	virtual bool Login(int32 LocalUserNum, const FOnlineAccountCredentials& AccountCredentials) override
	{
		TSharedPtr<const FUniqueNetIdString> NewId = MakeShared<FUniqueNetIdOdinFleet>(FString::Printf(TEXT("OdinUser%d"), LocalUserNum));
		UserIds.Add(LocalUserNum, NewId);
		UserIdToLocalUserNum.Add(NewId->ToString(), LocalUserNum);

		auto Account = MakeShared<FUserOnlineAccountOdinFleet>(AccountCredentials.Id);
		UserAccounts.Add(*NewId, Account);

		TriggerOnLoginCompleteDelegates(LocalUserNum, true, *NewId, TEXT("Success"));

		return true; 
	}
	virtual bool Logout(int32 LocalUserNum) override 
	{ 
		UserIds.Remove(LocalUserNum);

		TriggerOnLogoutCompleteDelegates(LocalUserNum, true);

		return true;
	}
	virtual bool AutoLogin(int32 LocalUserNum) override 
	{ 
		FOnlineAccountCredentials AutoCreds;
		AutoCreds.Type = TEXT("Auto");
		AutoCreds.Id = FString::Printf(TEXT("AutoUser%d"), LocalUserNum);
		AutoCreds.Token = TEXT("None");

		return Login(LocalUserNum, AutoCreds);
	}

	// Inherited via IOnlineIdentity
	TSharedPtr<FUserOnlineAccount> GetUserAccount(const FUniqueNetId& UserId) const override;
	TArray<TSharedPtr<FUserOnlineAccount>> GetAllUserAccounts() const override;
	TMap<FString, int32> UserIdToLocalUserNum;
	FUniqueNetIdPtr CreateUniquePlayerId(uint8* Bytes, int32 Size) override;
	ELoginStatus::Type GetLoginStatus(const FUniqueNetId& UserId) const override;
	FString GetPlayerNickname(const FUniqueNetId& UserId) const override;
	void RevokeAuthToken(const FUniqueNetId& LocalUserId, const FOnRevokeAuthTokenCompleteDelegate& Delegate) override;
	void GetUserPrivilege(const FUniqueNetId& LocalUserId, EUserPrivileges::Type Privilege, const FOnGetUserPrivilegeCompleteDelegate& Delegate, EShowPrivilegeResolveUI ShowResolveUI) override;
	FString GetAuthType() const override;

	TMap<int32, TSharedPtr<const FUniqueNetId>> UserIds;
	TMap<FUniqueNetIdString, TSharedPtr<FUserOnlineAccount>> UserAccounts;
};
