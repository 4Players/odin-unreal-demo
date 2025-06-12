// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystemImpl.h"

class FOnlineSessionOdinFleet;
class FOnlineIdentityOdinFleet;

class FOnlineSubsystemOdinFleet : public FOnlineSubsystemImpl
{
public:
    FOnlineSubsystemOdinFleet(FName InInstanceName) : FOnlineSubsystemImpl(TEXT("OdinFleet"), InInstanceName) {}

    virtual IOnlineSessionPtr GetSessionInterface() const override { return SessionInterface; }
    virtual IOnlineIdentityPtr GetIdentityInterface() const override { return IdentityInterface; }

    bool Init() override;
    bool Shutdown() override;

private:
    TSharedPtr<IOnlineSession, ESPMode::ThreadSafe> SessionInterface;
    TSharedPtr<IOnlineIdentity, ESPMode::ThreadSafe> IdentityInterface;

    // Inherited via FOnlineSubsystemImpl
    IOnlineFriendsPtr GetFriendsInterface() const override;
    FString GetAppId() const override;
    FText GetOnlineServiceName() const override;
};