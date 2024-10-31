// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/InteractableBase.h"
#include "Barricade.generated.h"

/**
 * 
 */
UCLASS()
class OUTLINE_API ABarricade : public AInteractableBase
{
	GENERATED_BODY()

public:

	ABarricade();


protected:

	UPROPERTY(EditAnywhere, Category = "Outline Settings")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Outline Settings")
	int32 Cost;

	UPROPERTY(ReplicatedUsing = OnRep_BarricadeUsed)
	bool bIsUsed;

	UFUNCTION()
	void OnRep_BarricadeUsed();


public:
	
	UFUNCTION(BlueprintPure)
	int32 GetInteractableCost();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Use(class ACharacterBase* Player) override;
	
};
