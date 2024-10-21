// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OutlineGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class OUTLINE_API AOutlineGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:

	AOutlineGameModeBase();

protected:
	
	UPROPERTY(EditAnywhere, Category = "Outline Settings")
	TSubclassOf<class AZombieBase> ZombieClass;

	class AOutlineGameState* OutlineGameState;
	
	TArray<class AOutlinePlayerSpawnPoint*> PlayerSpawnPoints;
	TArray<class AOutlineZombieSpawnPoint*> ZombieSpawnPoints;
	FTimerHandle SpawnZombieTimerHandle;
	uint16 ZombiesRemaning;

	bool bHasLoadedSpawnPoints;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;

	void GatherPlayerSpawnPoints();
	void GatherZombieSpawnPoints();
	void SpawnZombie();
	void CalculateZombiesCount();
	
};
