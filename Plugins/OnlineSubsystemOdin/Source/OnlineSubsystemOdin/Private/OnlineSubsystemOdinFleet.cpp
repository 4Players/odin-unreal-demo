// Copyright Epic Games, Inc. All Rights Reserved.

#include "OnlineSubsystemOdinFleet.h"
#include "OnlineSessionOdinFleet.h"
#include "OnlineIdentityOdinFleet.h"

bool FOnlineSubsystemOdinFleet::Init()
{
    SessionInterface = MakeShared<FOnlineSessionOdinFleet>() ;
    IdentityInterface = MakeShared<FOnlineIdentityOdinFleet>();
    return true;
}

bool FOnlineSubsystemOdinFleet::Shutdown()
{
    SessionInterface = nullptr;
    IdentityInterface = nullptr;
    return true;
}