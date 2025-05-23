#pragma once

#include "Interfaces/OnlineSessionInterface.h"

class FOnlineSessionInfoOdinFleet : public FOnlineSessionInfo
{
public:
	FOnlineSessionInfoOdinFleet() : HostAddress(TEXT("127.0.0.1")) {}
	FOnlineSessionInfoOdinFleet(const FString& InAddress) : HostAddress(InAddress) {}

	virtual const uint8* GetBytes() const override { return nullptr; }
	virtual int32 GetSize() const override { return 0; }
	virtual bool IsValid() const override { return true; }
	virtual FString ToString() const override { return HostAddress; }

private:
	FString HostAddress;

	// Inherited via FOnlineSessionInfo
	FString ToDebugString() const override { return HostAddress; }
	const FUniqueNetId& GetSessionId() const override { return *FUniqueNetIdString::Create(HostAddress, TEXT("ODIN")); }
};