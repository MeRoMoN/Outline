// Fill out your copyright notice in the Description page of Project Settings.


#include "Outline/Public/Game/OutlineGameModeBase.h"
#include "Outline/Public/Game/OutlinePlayerSpawnPoint.h"
#include "Player/CharacterBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Game/OutlineZombieSpawnPoint.h"
#include "TimerManager.h"
#include "Zombies/ZombieBase.h"
#include "Game/OutlineGameState.h"

AOutlineGameModeBase::AOutlineGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Player/BP_CharacterBase"));
	if (PlayerPawnClassFinder.Succeeded())
	{
		DefaultPawnClass = PlayerPawnClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonPlayerController"));
	if (PlayerControllerClassFinder.Succeeded())
	{
		PlayerControllerClass = PlayerControllerClassFinder.Class;
	}

	bHasLoadedSpawnPoints = false;
	ZombiesRemaning = 0;
}

void AOutlineGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	OutlineGameState = GetGameState<AOutlineGameState>();
	CalculateZombiesCount();

	GatherZombieSpawnPoints();
	GetWorld()->GetTimerManager().SetTimer(SpawnZombieTimerHandle, this, &AOutlineGameModeBase::SpawnZombie, 2.f, true);
}

void AOutlineGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	if(!bHasLoadedSpawnPoints)
	{ 
		GatherPlayerSpawnPoints();
	}	

	for (AOutlinePlayerSpawnPoint* SpawnPoint : PlayerSpawnPoints)
	{
		if (!SpawnPoint->IsUsed())
		{
			FVector SpawnLocation = SpawnPoint->GetActorLocation();
			APawn* Pawn =NewPlayer->GetPawn();
			if (Pawn)
			{
				Pawn->SetActorLocation(SpawnLocation);
			}
			else
			{
				Pawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation, FRotator::ZeroRotator);
				if(Pawn)
					NewPlayer->Possess(Pawn);
			}

			SpawnPoint->SetUsed(true);
			return;
		}
	}
}

void AOutlineGameModeBase::GatherPlayerSpawnPoints()
{
	TArray<AActor*> TempActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOutlinePlayerSpawnPoint::StaticClass(), TempActors);

	for (AActor* TempActor : TempActors)
	{
		if (AOutlinePlayerSpawnPoint* SpawnPoint = Cast<AOutlinePlayerSpawnPoint>(TempActor))
		{
				PlayerSpawnPoints.Add(SpawnPoint);
		}
	}
	bHasLoadedSpawnPoints = true;
}

void AOutlineGameModeBase::GatherZombieSpawnPoints()
{
	TArray<AActor*> TempActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOutlineZombieSpawnPoint::StaticClass(), TempActors);

	for (AActor* TempActor : TempActors)
	{
		if (AOutlineZombieSpawnPoint* SpawnPoint = Cast<AOutlineZombieSpawnPoint>(TempActor))
		{
			ZombieSpawnPoints.Add(SpawnPoint);
		}
	}
}

void AOutlineGameModeBase::SpawnZombie()
{
	if (ZombiesRemaning > 0)
	{
		int RandomIndex = FMath::RandRange(0, ZombieSpawnPoints.Num() - 1);

		if (AOutlineZombieSpawnPoint* SpawnPoint = ZombieSpawnPoints[RandomIndex])
		{
			FVector Loc = SpawnPoint->GetActorLocation();
			FRotator Rot = SpawnPoint->GetActorRotation();
			if (AZombieBase* Zombie = GetWorld()->SpawnActor<AZombieBase>(ZombieClass, Loc, Rot))
			{
				--ZombiesRemaning;
			}
		}
	}
	else
	{
		GetWorld()->GetTimerManager().PauseTimer(SpawnZombieTimerHandle);
	}
}

void AOutlineGameModeBase::CalculateZombiesCount()
{
	uint16 RoundNumber = OutlineGameState->GetRoundNumber();
	//Some calculations are supposed to be happening here
	ZombiesRemaning = 5;
}
