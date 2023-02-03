// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "OdinUnrealSample.h"
#include "PhotonLBClient.h"
#include "demo_particle_common/Console.h"

UPhotonLBClient::UPhotonLBClient(const class FObjectInitializer& PCIP)
	: Super(PCIP),
	serverAddress(TEXT("ns.exitgames.com")),
	AppID(TEXT("<no-app-id>")),
	appVersion(TEXT("1.0")),
	mpClient(NULL),
	mpListener(NULL)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPhotonLBClient::BeginPlay(void)
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

void UPhotonLBClient::TickComponent(float DeltaSeconds, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaSeconds, TickType, ThisTickFunction);
	if(mpClient)
	{
		mpClient->service();
		mpListener->service();
	}
}

void UPhotonLBClient::NewGame(void)
{
	mpListener->createRoom();
}

void UPhotonLBClient::Leave(void)
{
	if(mpClient->getIsInGameRoom())
		mpClient->opLeaveRoom();
}

void UPhotonLBClient::JoinRoom(FString gameId)
{
	mpClient->opJoinRoom(TCHAR_TO_UTF8(*gameId));
}

void UPhotonLBClient::SetLocalPlayerPos(float x, float y, float z, float rotX, float rotY, float rotZ)
{
	mpListener->setLocalPlayerPos(x, y, z, rotX, rotY, rotZ);
}

void UPhotonLBClient::SendTriggerEvent(int id)
{
	mpListener->sendTriggerEvent(id);
}

void UPhotonLBClient::SetLocalPlayerRot(float x, float y, float z)
{
	mpListener->setLocalPlayerRot(x, y, z);
}

bool UPhotonLBClient::IsInLobby(void)
{
	return mpClient->getState() == ExitGames::LoadBalancing::PeerStates::JoinedLobby || mpClient->getState() == ExitGames::LoadBalancing::PeerStates::AuthenticatedComingFromGameserver;
}

bool UPhotonLBClient::IsInRoom(void)
{
	return mpClient->getState() == ExitGames::LoadBalancing::PeerStates::Joined;
}

#if WITH_EDITOR
void UPhotonLBClient::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property) ? e.Property->GetFName() : NAME_None;
}
#endif