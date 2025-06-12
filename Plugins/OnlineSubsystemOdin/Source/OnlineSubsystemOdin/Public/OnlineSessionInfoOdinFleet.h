#pragma once

#include "Interfaces/OnlineSessionInterface.h"

class FOdinNetId : public FUniqueNetIdString
{
public:
    FOdinNetId(const FString& InId)
        : FUniqueNetIdString(InId, FName(TEXT("OdinFleet")))
    {
    }

    FOdinNetId()
        : FOdinNetId(TEXT("any"))
    { }

    virtual ~FOdinNetId() override {}
};
#include "SocketSubsystem.h"

class FOnlineSessionInfoOdinFleet : public FOnlineSessionInfo
{
public:
    FOnlineSessionInfoOdinFleet(const FString& InIP, int32 InPort)
        : HostAddr(ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr())
    {
        bool bIsValid;
        HostAddr->SetIp(*InIP, bIsValid);
        HostAddr->SetPort(InPort);

        // Use IP:Port as a consistent session ID string
        FString SessionIdStr = FString::Printf(TEXT("%s:%d"), *InIP, InPort);
        SessionIdRef = MakeShared<FOdinNetId>(SessionIdStr);

        // Generate byte array for comparisons (e.g., SHA1 hash)
        FString RawData = InIP + FString::FromInt(InPort);
        FSHA1 Sha;
        Sha.Update((const uint8*)TCHAR_TO_ANSI(*RawData), RawData.Len());
        Sha.Final();

        SessionBytes.SetNum(20); // SHA1 is 20 bytes
        Sha.GetHash(SessionBytes.GetData());
    }

    TSharedPtr<const FInternetAddr> GetHostAddr() const
    {
        return HostAddr;
    }

    // Inherited via FOnlineSessionInfo
    const uint8* GetBytes() const override;
    int32 GetSize() const override;
    bool IsValid() const override;
    FString ToString() const override;
    FString ToDebugString() const override;
    const FUniqueNetId& GetSessionId() const override;

private:
    TSharedPtr<FInternetAddr> HostAddr;
    TSharedRef<const FOdinNetId> SessionIdRef;
    TArray<uint8> SessionBytes;
};