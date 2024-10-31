// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableBase.generated.h"


UCLASS()
class OUTLINE_API AInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	
	UPROPERTY(EditAnywhere, Category = "Outline Settings")
	FText Name;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	
	UFUNCTION(BlueprintPure)
	FText GetInteractableName();

	virtual void Use(class ACharacterBase* Player);


};
