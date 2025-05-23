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
    FOnlineSubsystemOdinFleet() : FOnlineSubsystemImpl(TEXT("OdinFleet"), TEXT("OdinFleetInstance")) {}

    virtual IOnlineSessionPtr GetSessionInterface() const override { return StaticCastSharedPtr<IOnlineSession>(SessionInterface); }
    virtual IOnlineIdentityPtr GetIdentityInterface() const override { return StaticCastSharedPtr<IOnlineIdentity>(IdentityInterface); }

    bool Init() override;
    bool Shutdown() override;

private:
    TSharedPtr<FOnlineSessionOdinFleet> SessionInterface;
    TSharedPtr<FOnlineIdentityOdinFleet> IdentityInterface; 
};