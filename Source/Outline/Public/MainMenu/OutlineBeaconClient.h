// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "Outline/Public/MainMenu/OutlineBeaconHostObject.h"
#include "OutlineBeaconClient.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyInfoUpdated, FLobbyInfo, LobbyInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectedToHost, bool, Connected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDisconnected);

UCLASS()
class OUTLINE_API AOutlineBeaconClient : public AOnlineBeaconClient
{
	GENERATED_BODY()

public:

	AOutlineBeaconClient();

protected:

	UFUNCTION(BlueprintCallable)
	bool ConnectToServer(const FString& Address);

	UFUNCTION(BlueprintCallable)
	void LeaveLobby();

	UPROPERTY(BlueprintAssignable)
	FOnConnectedToHost OnConnectedToHost;

	UPROPERTY(BlueprintAssignable)
	FOnDisconnected OnDisconnected;

	UPROPERTY(BlueprintAssignable)
	FOnLobbyInfoUpdated OnLobbyInfoUpdated;

private:

	virtual void OnFailure() override;
	virtual void OnConnected() override;

public:

	UFUNCTION(Client, Reliable)
	void Client_OnDisconnected();
	virtual void Client_OnDisconnected_Implementation();

	UFUNCTION(Client, Reliable)
	void Client_OnLobbyUpdated(FLobbyInfo LobbyInfo);
	virtual void Client_OnLobbyUpdated_Implementation(FLobbyInfo LobbyInfo);

};
