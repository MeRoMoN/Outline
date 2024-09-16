// Fill out your copyright notice in the Description page of Project Settings.


#include "Outline/Public/MainMenu/OutlineBeaconHostObject.h"
#include "Outline/Public/MainMenu/OutlineBeaconClient.h"
#include "Outline/Public/MainMenu/OutlineMainMenuGameMode.h"
#include <OnlineBeaconHost.h>

AOutlineBeaconHostObject::AOutlineBeaconHostObject()
{
	ClientBeaconActorClass = AOutlineBeaconClient::StaticClass();
	BeaconTypeName = ClientBeaconActorClass->GetName();

	Http = &FHttpModule::Get();
	ServerID = -1;
}

void AOutlineBeaconHostObject::UpdateLobbyInfo(FLobbyInfo NewLobbyInfo)
{
	LobbyInfo.MapImage = NewLobbyInfo.MapImage;
	UpdateClientLobbyInfo();
}

void AOutlineBeaconHostObject::OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
{
	if (Success)
	{
		ServerID = FCString::Atoi(*Response->GetContentAsString());
		UE_LOG(LogTemp, Warning, TEXT("HTTP Request Success: %d"), ServerID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HTTP Request FAILED"));
	}
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

	PostServerEntry();
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

	DeleteServerEntry();
}

void AOutlineBeaconHostObject::OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection)
{
	Super::OnClientConnected(NewClientActor, ClientConnection);
	
	if (NewClientActor)
	{
		FString PlayerName = FString("Player ");
		PlayerName.Append(FString::FromInt(LobbyInfo.PlayerList.Num()));
		LobbyInfo.PlayerList.Add(PlayerName);

		if (AOutlineBeaconClient* OutlineClient = Cast<AOutlineBeaconClient>(NewClientActor))
		{
			OutlineClient->SetPlayerName(PlayerName);
		}

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

void AOutlineBeaconHostObject::PostServerEntry()
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetNumberField("ServerID", 0);
	JsonObject->SetStringField("IPAddress", "127.0.0.1");
	JsonObject->SetStringField("ServerName", "Test server");
	JsonObject->SetStringField("MapName", "TestName");
	JsonObject->SetNumberField("CurrentPlayers", 1);
	JsonObject->SetNumberField("MaxPlayers", 5);

	FString JsonString;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AOutlineBeaconHostObject::OnProcessRequestComplete);

	Request->SetURL("https://localhost:44349/api/Host");
	Request->SetVerb("POST");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(JsonString);

	Request->ProcessRequest();
}

void AOutlineBeaconHostObject::DeleteServerEntry()
{
	if (ServerID != -1)
	{
		TSharedRef<IHttpRequest> Request = Http->CreateRequest();

		Request->SetURL("https://localhost:44349/api/Host/" + FString::FromInt(ServerID));
		Request->SetVerb("DELETE");
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));;

		Request->ProcessRequest();
	}
}

FLobbyInfo AOutlineBeaconHostObject::GetLobbyInfo() const
{
	return LobbyInfo;
}

void AOutlineBeaconHostObject::SendMessageToLobbyChat(const FText& ChatMessage)
{
	OnHostChatMessageRecieved.Broadcast(ChatMessage);
	for (AOnlineBeaconClient* ClientBeacon : ClientActors)
	{
		if (AOutlineBeaconClient* Client = Cast<AOutlineBeaconClient>(ClientBeacon))
		{
			Client->Client_OnChatMessageReceived(ChatMessage);
		}
	}
}
