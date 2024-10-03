#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Http.h"
#include "OutlineGameInstanceBase.generated.h"

USTRUCT(BlueprintType)
struct FMapInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapURL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapDescription;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTexture2D* MapImage;
};

USTRUCT(BlueprintType)
struct FServerData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ServerID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString IPAddress;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ServerName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MapName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentPlayers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxPlayers;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnServersListReceived);

UCLASS()
class OUTLINE_API UOutlineGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:

	UOutlineGameInstanceBase();

protected:

	UPROPERTY(BlueprintAssignable)
	FOnServersListReceived OnServersListReceived;

	UFUNCTION(BlueprintCallable)
	void GoToMap(FString MapURL);

	UFUNCTION(BlueprintCallable)
	void RetrieveServerList();

	UFUNCTION(BlueprintPure)
	TArray<FServerData>& GetServerList();


	FHttpModule* Http;
	TArray<FServerData> ServerList;

	void OnServerListProcessRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success);
};