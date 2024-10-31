// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/InteractableBase.h"
#include "Player/CharacterBase.h"

// Sets default values
AInteractableBase::AInteractableBase()
{
	Name = FText::FromString("Interactable item");
}

// Called when the game starts or when spawned
void AInteractableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FText AInteractableBase::GetInteractableName()
{
	return Name;
}

void AInteractableBase::Use(ACharacterBase* Player)
{

}

