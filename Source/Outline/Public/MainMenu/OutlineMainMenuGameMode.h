// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OutlineMainMenuGameMode.generated.h"

class AOutlineBeaconHostObject;
class AOnlineBeaconHost;

/**
 * 
 */
UCLASS()
class OUTLINE_API AOutlineMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AOutlineMainMenuGameMode();
	
protected:

	UFUNCTION(BlueprintCallable)
	bool CreateHostBeacon();

	UFUNCTION(BlueprintPure)
	AOutlineBeaconHostObject* GetBeaconHostObject();


	AOutlineBeaconHostObject* HostObject;

};
