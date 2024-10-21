// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/OutlineGameState.h"

AOutlineGameState::AOutlineGameState()
{
	RoundNumber = 1;
	ZombiesOnMap = 0;
}

uint16 AOutlineGameState::GetRoundNumber()
{
	return RoundNumber;
}

void AOutlineGameState::IncrementRoundNumber()
{
	++RoundNumber;
}
