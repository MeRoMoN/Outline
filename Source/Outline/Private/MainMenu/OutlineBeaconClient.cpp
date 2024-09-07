// Fill out your copyright notice in the Description page of Project Settings.


#include "Outline/Public/MainMenu/OutlineBeaconClient.h"

AOutlineBeaconClient::AOutlineBeaconClient()
{
	PlayerName = "Player";
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

void AOutlineBeaconClient::Client_OnChatMessageReceived_Implementation(const FText& Message)
{
	OnChatMessageRecieved.Broadcast(Message);
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

void AOutlineBeaconClient::SendChatMesssage(const FText& Message)
{
	Server_SendChatMessage(Message);
}

bool AOutlineBeaconClient::Server_SendChatMessage_Validate(const FText& Message)
{
	return true;
}

void AOutlineBeaconClient::Server_SendChatMessage_Implementation(const FText& Message)
{
	FString ChatMessage = PlayerName + ": " + Message.ToString();

	if (AOutlineBeaconHostObject* Host = Cast<AOutlineBeaconHostObject>(BeaconOwner))
	{
		Host->SendMessageToLobbyChat(FText::FromString(ChatMessage));
	}
}

void AOutlineBeaconClient::SetPlayerName(const FString& NewPlayerName)
{
	PlayerName = NewPlayerName;
}

FString AOutlineBeaconClient::GetPlayerName() const
{
	return PlayerName;
}
