// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Barricade.h"
#include <Components/StaticMeshComponent.h>
#include <Net/UnrealNetwork.h>

ABarricade::ABarricade()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");

	RootComponent = Mesh;
	Name = FText::FromString("Barricade");
	Cost = 1000;
	SetReplicates(true);
	bIsUsed = false;
}

void ABarricade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABarricade, bIsUsed);
}

void ABarricade::OnRep_BarricadeUsed()
{
	SetActorEnableCollision(false);
}

int32 ABarricade::GetInteractableCost()
{
	return Cost;
}

void ABarricade::Use(ACharacterBase* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("USE IN BARRICADE"));
	bIsUsed = true;
	OnRep_BarricadeUsed();
}
