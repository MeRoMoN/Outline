// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CharacterBase.h"
#include "Outline/OutlineProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Interactables/InteractableBase.h"


ACharacterBase::ACharacterBase()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	InteractableObject = nullptr;
	InteractRate = 0.2f;
	InteractRange = 300.f;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if(GetLocalRole() != ENetRole::ROLE_SimulatedProxy)
		GetWorld()->GetTimerManager().SetTimer(InteractTimerHandle, this, &ACharacterBase::PerformInteractionTrace, InteractRate,true);	
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACharacterBase::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACharacterBase::Look);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACharacterBase::Interact);
	}
}

void ACharacterBase::Server_Interact_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Server_Interact"));
	PerformInteractionTrace();
	if (InteractableObject)
	{
		InteractableObject->Use(this);
	}
}

void ACharacterBase::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ACharacterBase::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACharacterBase::PerformInteractionTrace()
{
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector Rot = GetBaseAimRotation().Vector();
	FVector End = Start + Rot * InteractRange;

	FHitResult HitResult;
	FCollisionObjectQueryParams CollisionQuery;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, CollisionQuery, CollisionParams);

	if (AInteractableBase* HitInteractable = Cast<AInteractableBase>(HitResult.GetActor()))
	{
		if (InteractableObject == nullptr)
		{
			InteractableObject = HitInteractable;
			UE_LOG(LogTemp, Warning, TEXT("HIT ACTOR %s"), *HitResult.GetActor()->GetName());
			OnInteractableHit.Broadcast(InteractableObject);
		}
	}
	else if(InteractableObject)
	{
		InteractableObject = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("THERE'S NOTHING IMPORTANT TO INTERACT WITH"));
		OnInteractableHit.Broadcast(nullptr);
	}
}

void ACharacterBase::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("INTERACT"));

	if (InteractableObject)
	{
		if(HasAuthority())
			InteractableObject->Use(this);
		else
			Server_Interact();
	}
}

