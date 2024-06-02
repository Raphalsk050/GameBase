// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DXCoreWorldSubsystem.h"

#include "DXCore.h"
#include "EngineUtils.h"
#include "Core/DXWorldSettings.h"

bool UDXCoreWorldSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

ACosmeticEventManager* UDXCoreWorldSubsystem::GetCosmeticEventManager()
{
	if((!_cosmeticEventManager || !_cosmeticEventManager->IsValidLowLevel()))
	{
		if(GetWorld()->GetNetMode() < NM_Client)
		{
			UE_LOG(DXCore, Warning, TEXT("CosmeticEventManager was not valid, spawning a new one as Server."));
			_cosmeticEventManager = GetWorld()->SpawnActor<ACosmeticEventManager>();
		}
		else
		{
			//If this is client we need find the already spawned and replicated cosmetic event manager from the server and assign it to our variable
			for (TActorIterator<ACosmeticEventManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				_cosmeticEventManager = *ActorItr;
				break;
			}
			
			if(_cosmeticEventManager && _cosmeticEventManager->IsValidLowLevel())
			{
				UE_LOG(DXCore, Log, TEXT("Found CosmeticEventManger from server and assigned it to client variable."));
			}
			else
			{
				UE_LOG(DXCore, Error, TEXT("CosmeticEventManager was not found, could not find one spawned from the server."));
			}
		}
	}
	return _cosmeticEventManager;
}

ELevelPlayState UDXCoreWorldSubsystem::GetLevelPlayState()
{
	if (UWorld* world = GetWorld())
	{
		ADXWorldSettings* ws = Cast<ADXWorldSettings>(world->GetWorldSettings());
		return ws->GetLevelPlayState();
	}
	return ELevelPlayState::LPS_Final;
}

FName UDXCoreWorldSubsystem::GetCurrentCheckpoint() 
{
	if (UWorld* world = GetWorld())
	{
		ADXWorldSettings* ws = Cast<ADXWorldSettings>(world->GetWorldSettings());
		return ws->GetCurrentCheckpoint();
	}
	return FName();
}

uint8 UDXCoreWorldSubsystem::GetCurrentCheckpointOrder()
{
	if (UWorld* world = GetWorld())
	{
		ADXWorldSettings* ws = Cast<ADXWorldSettings>(world->GetWorldSettings());
		return ws->GetCurrentCheckpointOrder();
	}
	return 0;
}

ACheckpoint* UDXCoreWorldSubsystem::GetCurrentCheckpointActor()
{
	if (UWorld* world = GetWorld())
	{
		ADXWorldSettings* ws = Cast<ADXWorldSettings>(world->GetWorldSettings());
		return Cast<ACheckpoint>(ws->GetCurrentCheckpointActor());
	}
	return nullptr;
}

void UDXCoreWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if((!_cosmeticEventManager || !_cosmeticEventManager->IsValidLowLevel()))
	{
		if(GetWorld()->GetNetMode() < NM_Client)
		{
			UE_LOG(DXCore, Warning, TEXT("CosmeticEventManager was not valid, spawning a new one as Server."));
			_cosmeticEventManager = GetWorld()->SpawnActor<ACosmeticEventManager>();
		}
	}
}
