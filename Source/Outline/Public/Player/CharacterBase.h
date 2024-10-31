// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class AInteractableBase; 

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableHit, AInteractableBase*, InteractableObject);

UCLASS()
class OUTLINE_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractableHit OnInteractableHit;

	ACharacterBase();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float InteractRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float InteractRange;


	FTimerHandle InteractTimerHandle;
	AInteractableBase* InteractableObject;

protected:

	virtual void BeginPlay();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	
	UFUNCTION(Server, unreliable)
	void Server_Interact();
	void Server_Interact_Implementation();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void PerformInteractionTrace();
	void Interact();

protected:

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:

	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};
