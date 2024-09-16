// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHostObject.h"
#include "Http.h"
#include "OutlineBeaconHostObject.generated.h"

USTRUCT(BlueprintType)
struct FLobbyInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	class UTexture2D* MapImage;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> PlayerList;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHostLobbyInfoUpdated, FLobbyInfo, LobbyInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHostChatMessageRecieved, const FText&, Message);


UCLASS()
class OUTLINE_API AOutlineBeaconHostObject : public AOnlineBeaconHostObject
{
	GENERATED_BODY()

public:

	AOutlineBeaconHostObject();

protected:

	UPROPERTY(BlueprintAssignable)
	FOnHostLobbyInfoUpdated OnHostLobbyInfoUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnHostChatMessageRecieved OnHostChatMessageRecieved;

	UFUNCTION(BlueprintCallable)
	void UpdateLobbyInfo(FLobbyInfo NewLobbyInfo);


	FLobbyInfo LobbyInfo;

	FHttpModule* Http;
	int ServerID;

	void OnProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);
	void UpdateClientLobbyInfo();
	virtual void BeginPlay() override;

private:

	UFUNCTION(BlueprintCallable)
	void ShutdownServer();

	virtual void OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection) override;
	virtual void NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor) override;
	virtual void DisconnectClient(AOnlineBeaconClient* ClientActor) override;

	void PostServerEntry();
	void DeleteServerEntry();


public:

	UFUNCTION(BlueprintPure)
	FLobbyInfo GetLobbyInfo() const;

	UFUNCTION(BlueprintCallable)
	void SendMessageToLobbyChat(const FText& ChatMessage);
};
