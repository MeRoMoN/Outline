// Fill out your copyright notice in the Description page of Project Settings.


#include "Outline/Public/MainMenu/OutlineMainMenuGameMode.h"
#include "Outline/Public/MainMenu/OutlineBeaconHostObject.h"
#include <OnlineBeaconHost.h>


AOutlineMainMenuGameMode::AOutlineMainMenuGameMode()
{
	HostObject = nullptr;
}

bool AOutlineMainMenuGameMode::CreateHostBeacon()
{
	if (AOnlineBeaconHost* Host = GetWorld()->SpawnActor<AOnlineBeaconHost>(AOnlineBeaconHost::StaticClass()))
	{
		if (Host->InitHost())
		{
			Host->PauseBeaconRequests(false);

			HostObject = GetWorld()->SpawnActor<AOutlineBeaconHostObject>(AOutlineBeaconHostObject::StaticClass());

			if (HostObject)
			{
				Host->RegisterHost(HostObject);
				return true;
			}

		}
	}
	return false;
}

AOutlineBeaconHostObject* AOutlineMainMenuGameMode::GetBeaconHostObject()
{
	return HostObject;
}
