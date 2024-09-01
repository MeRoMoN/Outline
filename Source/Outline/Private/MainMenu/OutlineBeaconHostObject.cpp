// Fill out your copyright notice in the Description page of Project Settings.


#include "Outline/Public/MainMenu/OutlineBeaconHostObject.h"
#include "Outline/Public/MainMenu/OutlineBeaconClient.h"
#include "Outline/Public/MainMenu/OutlineMainMenuGameMode.h"
#include <OnlineBeaconHost.h>

AOutlineBeaconHostObject::AOutlineBeaconHostObject()
{
	ClientBeaconActorClass = AOutlineBeaconClient::StaticClass();
	BeaconTypeName = ClientBeaconActorClass->GetName();
}

void AOutlineBeaconHostObject::UpdateLobbyInfo(FLobbyInfo NewLobbyInfo)
{
	LobbyInfo.MapImage = NewLobbyInfo.MapImage;
	UpdateClientLobbyInfo();
}

void AOutlineBeaconHostObject::UpdateClientLobbyInfo()
{
	for (AOnlineBeaconClient* ClientBeacon : ClientActors)
	{
		if (AOutlineBeaconClient* Client = Cast<AOutlineBeaconClient>(ClientBeacon))
		{
			Client->Client_OnLobbyUpdated(LobbyInfo);
		}
	}
}

void AOutlineBeaconHostObject::BeginPlay()
{
	Super::BeginPlay();
	LobbyInfo.PlayerList.Add(FString("Host Player"));
}

void AOutlineBeaconHostObject::ShutdownServer()
{
	if (AOnlineBeaconHost* Host = Cast<AOnlineBeaconHost>(GetOwner()))
	{
		Host->UnregisterHost(BeaconTypeName);
		Host->DestroyBeacon();
	}

	UE_LOG(LogTemp, Warning, L"DISCONNECTING ALL CLIENTS");
	Unregister();
}

void AOutlineBeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection)
{
	Super::OnClientConnected(NewClientActor, ClientConnection);
	
	if (NewClientActor)
	{
		FString PlayerName = FString("Player ");
		PlayerName.Append(FString::FromInt(LobbyInfo.PlayerList.Num()));
		LobbyInfo.PlayerList.Add(PlayerName);

		OnHostLobbyInfoUpdated.Broadcast(LobbyInfo);

		UE_LOG(LogTemp, Warning, L"CONNECTED CLIENT VALID");
		UpdateClientLobbyInfo();
	}
	else
	{
		UE_LOG(LogTemp, Warning, L"CONNECTED CLIENT INVALID");
	}
}

void AOutlineBeaconHostObject::NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor)
{
	Super::NotifyClientDisconnected(LeavingClientActor);
	UE_LOG(LogTemp, Warning, L"CLIENT HAS DISCONNECTED");

	LobbyInfo.PlayerList.Pop(true);
	
	OnHostLobbyInfoUpdated.Broadcast(LobbyInfo);
	UpdateClientLobbyInfo();
}

void AOutlineBeaconHostObject::DisconnectClient(AOnlineBeaconClient* ClientActor)
{
	AOnlineBeaconHost* BeaconHost = Cast<AOnlineBeaconHost>(GetOwner());
	if (BeaconHost)
	{
		if (AOutlineBeaconClient* Client = Cast<AOutlineBeaconClient>(ClientActor))
		{
			UE_LOG(LogTemp, Warning, L"DISCONNECTING CLIENT %s", *ClientActor->GetName());
			Client->Client_OnDisconnected();
		}

		BeaconHost->DisconnectClient(ClientActor);
	}
}

FLobbyInfo AOutlineBeaconHostObject::GetLobbyInfo() const
{
	return LobbyInfo;
}
