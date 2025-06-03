#include "OnlineSessionInfoOdinFleet.h"

const uint8* FOnlineSessionInfoOdinFleet::GetBytes() const
{
    return SessionBytes.GetData();
}

int32 FOnlineSessionInfoOdinFleet::GetSize() const
{
    return SessionBytes.Num();
}

bool FOnlineSessionInfoOdinFleet::IsValid() const
{
    return HostAddr.IsValid() && HostAddr->IsValid();
}

FString FOnlineSessionInfoOdinFleet::ToString() const
{
    return SessionIdRef->ToString();
}

FString FOnlineSessionInfoOdinFleet::ToDebugString() const
{
    return FString::Printf(TEXT("OdinFleetSession IP=%s"), *HostAddr->ToString(true));
}

const FUniqueNetId& FOnlineSessionInfoOdinFleet::GetSessionId() const
{
    return *SessionIdRef;
}
