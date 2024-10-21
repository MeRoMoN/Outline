// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OutlineGameState.generated.h"

/**
 * 
 */
UCLASS()
class OUTLINE_API AOutlineGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AOutlineGameState();

protected:

	uint16 RoundNumber;
	uint16 ZombiesOnMap;


public:

	uint16 GetRoundNumber();
	void IncrementRoundNumber();
};
