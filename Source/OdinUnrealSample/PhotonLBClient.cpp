// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "OdinUnrealSample.h"
#include "PhotonLBClient.h"
#include "demo_particle_common/Console.h"

APhotonLBClient::APhotonLBClient(const class FObjectInitializer& PCIP)
	: Super(PCIP),
	serverAddress(TEXT("ns.exitgames.com")),
	AppID(TEXT("<no-app-id>")),
	appVersion(TEXT("1.0")),
	mpClient(NULL),
	mpListener(NULL)
{
	PrimaryActorTick.bCanEverTick = true;
}

void APhotonLBClient::BeginPlay(void)
{
	Super::BeginPlay();
	srand(GETTIMEMS());
	mpListener = new LoadBalancingListener(this);
	mpClient = new ExitGames::LoadBalancing::Client(*mpListener, TCHAR_TO_UTF8(*AppID), TCHAR_TO_UTF8(*appVersion));
	mpListener->setLBC(mpClient);

	automove = mpListener->getAutomove();
	useGroups = mpListener->getUseGroups();

	Console::get().writeLine(L"Connecting...");
	Console::get().writeLine(L"appID is set to " + ExitGames::Common::JString(TCHAR_TO_UTF8(*AppID)));
	mpClient->connect(ExitGames::LoadBalancing::AuthenticationValues().setUserID(ExitGames::Common::JString(L"UR") + GETTIMEMS())/*, ExitGames::Common::JString(L"UR") + GETTIMEMS(), TCHAR_TO_UTF8(*serverAddress)*/);

}

void APhotonLBClient::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(mpClient)
	{
		mpClient->service();
		mpListener->service();
	}
}

void APhotonLBClient::RandomizeColor(void)
{
	mpListener->changeRandomColor();
}

void APhotonLBClient::NextGridSize(void)
{
	mpListener->nextGridSize();
}

void APhotonLBClient::NewGame(void)
{
	mpListener->createRoom();
}

void APhotonLBClient::Leave(void)
{
	if(mpClient->getIsInGameRoom())
		mpClient->opLeaveRoom();
}

void APhotonLBClient::JoinRoom(FString gameId)
{
	mpClient->opJoinRoom(TCHAR_TO_UTF8(*gameId));
}

void APhotonLBClient::SetLocalPlayerPos(float x, float y, float z, float rotX, float rotY, float rotZ)
{
	mpListener->setLocalPlayerPos(x, y, z, rotX, rotY, rotZ);
}

void APhotonLBClient::SetLocalPlayerRot(float x, float y, float z)
{
	mpListener->setLocalPlayerRot(x, y, z);
}

void APhotonLBClient::SetAutomove(bool am)
{
	mpListener->setAutomove(automove=am);
}

void APhotonLBClient::SetUseGroups(bool ug)
{
	mpListener->setUseGroups(useGroups=ug);
}

bool APhotonLBClient::IsInLobby(void)
{
	return mpClient->getState() == ExitGames::LoadBalancing::PeerStates::JoinedLobby || mpClient->getState() == ExitGames::LoadBalancing::PeerStates::AuthenticatedComingFromGameserver;
}

bool APhotonLBClient::IsInRoom(void)
{
	return mpClient->getState() == ExitGames::LoadBalancing::PeerStates::Joined;
}

#if WITH_EDITOR
void APhotonLBClient::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property) ? e.Property->GetFName() : NAME_None;
	if(PropertyName == GET_MEMBER_NAME_CHECKED(APhotonLBClient, automove))
		SetAutomove(automove);
	else if(PropertyName == GET_MEMBER_NAME_CHECKED(APhotonLBClient, useGroups))
		SetUseGroups(useGroups);
}
#endif