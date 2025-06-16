#pragma once

#include "Interfaces/OnlineSessionInterface.h"
#include "SocketSubsystem.h"
#include "Templates/SharedPointer.h"

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

class FOnlineSessionInfoOdinFleet : public FOnlineSessionInfo
{
public:
	FOnlineSessionInfoOdinFleet(const FString& InIP, int32 InPort)
		: HostAddr(ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr())
		, SessionIdRef(MakeShared<FOdinNetId>(InIP + FString::FromInt(InPort)))
    {
        /*auto socketss = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
        HostAddr = socketss->CreateInternetAddr();*/
        bool bIsValid;
        HostAddr->SetIp(*InIP, bIsValid);
        HostAddr->SetPort(InPort);

        // Use IP:Port as a consistent session ID string
        /*FString SessionIdStr = FString::Printf(TEXT("%s:%d"), *InIP, InPort);
        SessionIdRef = MakeShared<FOdinNetId>(SessionIdStr);*/

        // Generate byte array for comparisons (e.g., SHA1 hash)
        FString RawData = InIP + FString::FromInt(InPort);
        FSHA1 Sha;
        Sha.Update((const uint8*)TCHAR_TO_ANSI(*RawData), RawData.Len());
        Sha.Final();

        SessionBytes.SetNum(20); // SHA1 is 20 bytes
        Sha.GetHash(SessionBytes.GetData());
    }

    TSharedPtr<FInternetAddr> GetHostAddr() const
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
    TSharedRef<FInternetAddr> HostAddr;

private:
    TSharedRef<FOdinNetId> SessionIdRef;
    TArray<uint8> SessionBytes;
};