// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "OutlinePlayerSpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class OUTLINE_API AOutlinePlayerSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:

	AOutlinePlayerSpawnPoint();

protected:

	bool bUsedSpawnPoint;

public:
	
	bool IsUsed();
	void SetUsed(bool bUsed);


	
};
