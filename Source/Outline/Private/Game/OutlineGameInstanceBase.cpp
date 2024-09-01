// Fill out your copyright notice in the Description page of Project Settings.


#include "Outline/Public/Game/OutlineGameInstanceBase.h"


UOutlineGameInstanceBase::UOutlineGameInstanceBase()
{

}

void UOutlineGameInstanceBase::GoToMap(FString MapURL)
{
	GetWorld()->ServerTravel(MapURL);
}

