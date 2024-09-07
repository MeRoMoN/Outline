// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "Outline/Public/MainMenu/OutlineBeaconHostObject.h"
#include "OutlineBeaconClient.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyInfoUpdated, FLobbyInfo, LobbyInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectedToHost, bool, Connected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatMessageRecieved, const FText&, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisconnected);

UCLASS()
class OUTLINE_API AOutlineBeaconClient : public AOnlineBeaconClient
{
	GENERATED_BODY()

public:

	AOutlineBeaconClient();


	UPROPERTY(BlueprintAssignable)
	FOnConnectedToHost OnConnectedToHost;

	UPROPERTY(BlueprintAssignable)
	FOnDisconnected OnDisconnected;

	UPROPERTY(BlueprintAssignable)
	FOnLobbyInfoUpdated OnLobbyInfoUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnChatMessageRecieved OnChatMessageRecieved;

private:

	virtual void OnFailure() override;
	virtual void OnConnected() override;

	FString PlayerName;

protected:

	UFUNCTION(BlueprintCallable)
	bool ConnectToServer(const FString& Address);

	UFUNCTION(BlueprintCallable)
	void LeaveLobby();

	UFUNCTION(BlueprintCallable)
	void SendChatMesssage(const FText& Message);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendChatMessage(const FText& Message);
	bool Server_SendChatMessage_Validate(const FText& Message);
	void Server_SendChatMessage_Implementation(const FText& Message);

public:

	UFUNCTION(Client, Reliable)
	void Client_OnDisconnected();
	virtual void Client_OnDisconnected_Implementation();

	UFUNCTION(Client, Reliable)
	void Client_OnLobbyUpdated(FLobbyInfo LobbyInfo);
	virtual void Client_OnLobbyUpdated_Implementation(FLobbyInfo LobbyInfo);

	UFUNCTION(Client, Reliable)
	void Client_OnChatMessageReceived(const FText& Message);
	void Client_OnChatMessageReceived_Implementation(const FText& Message);

	void SetPlayerName(const FString& NewPlayerName);
	FString GetPlayerName() const;

};
