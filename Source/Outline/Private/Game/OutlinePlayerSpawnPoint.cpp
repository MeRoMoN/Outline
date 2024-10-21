// Fill out your copyright notice in the Description page of Project Settings.


#include "Outline/Public/Game/OutlinePlayerSpawnPoint.h"

AOutlinePlayerSpawnPoint::AOutlinePlayerSpawnPoint()
{

}

bool AOutlinePlayerSpawnPoint::IsUsed()
{
	return bUsedSpawnPoint;
}

void AOutlinePlayerSpawnPoint::SetUsed(bool bUsed)
{
	bUsedSpawnPoint = bUsed;
}
