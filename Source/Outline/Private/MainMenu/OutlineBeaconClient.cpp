// Fill out your copyright notice in the Description page of Project Settings.


#include "Outline/Public/MainMenu/OutlineBeaconClient.h"

AOutlineBeaconClient::AOutlineBeaconClient()
{

}

void AOutlineBeaconClient::OnFailure()
{
	Super::OnFailure();
	UE_LOG(LogTemp, Warning, L"CLIENT FAILED TO CONNECT TO HOST BEACON");
	OnConnectedToHost.Broadcast(false);
}

void AOutlineBeaconClient::OnConnected()
{
	Super::OnConnected();
	UE_LOG(LogTemp, Warning, L"CLIENT CONNECTED TO HOST BEACON");
	OnConnectedToHost.Broadcast(true);
}

void AOutlineBeaconClient::Client_OnDisconnected_Implementation()
{
	UE_LOG(LogTemp, Warning, L"DISCONNECTED");
	OnDisconnected.Broadcast();
}

void AOutlineBeaconClient::Client_OnLobbyUpdated_Implementation(FLobbyInfo LobbyInfo)
{
	OnLobbyInfoUpdated.Broadcast(LobbyInfo);
}

bool AOutlineBeaconClient::ConnectToServer(const FString& Address)
{
	FURL Destination = FURL(nullptr, *Address, TRAVEL_Absolute);
	Destination.Port = 7787;
	return InitClient(Destination);
}

void AOutlineBeaconClient::LeaveLobby()
{
	DestroyBeacon();
}

