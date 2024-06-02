// Fill out your copyright notice in the Description page of Project Settings.


#include "CosmeticEventSystem/CosmeticEventManager.h"

#include "DXCore.h"
#include "DXCoreImportantTypes.h"

// Sets default values
ACosmeticEventManager::ACosmeticEventManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ACosmeticEventManager::LocalTriggerInstantCosmeticEvent(TSubclassOf<UInstantCosmeticEvent> CosmeticEvent,
	const FCosmeticEventData& parameters)
{
	InternalTriggerInstantCosmeticEvent(CosmeticEvent, parameters);
}

void ACosmeticEventManager::AuthTriggerInstantCosmeticEvent(TSubclassOf<UInstantCosmeticEvent> CosmeticEvent,
	const FCosmeticEventData& parameters, const TArray<APlayerController*>& clientsToIgnore)
{
	//check if this is server or authority
	if(HasAuthority())
	{
		MulticastTriggerInstantCosmeticEvent(CosmeticEvent, parameters,clientsToIgnore);
	}
	else
	{
		UE_LOG(DXCore, Error, TEXT("Tried triggering an AuthInstantCosmeticEvent but this is not the server."));
	}
}

ALatentCosmeticEvent* ACosmeticEventManager::LocalTriggerLatentCosmeticEvent(
	TSubclassOf<ALatentCosmeticEvent> CosmeticEvent, const FCosmeticEventData& parameters, FTransform SpawnTransform, int32& OutLatentCosmeticEventID)
{
	const int32 id = GetNextLatentCosmeticEventID();
	OutLatentCosmeticEventID = id;
	return InternalTriggerLatentCosmeticEvent(CosmeticEvent, parameters, true, SpawnTransform, id);
}

ALatentCosmeticEvent* ACosmeticEventManager::AuthTriggerLatentCosmeticEvent(
	TSubclassOf<ALatentCosmeticEvent> CosmeticEvent, const FCosmeticEventData& parameters, const FTransform& SpawnTransform, int32& OutLatentCosmeticEventID, int32 InputLatentCosmeticEventID)
{
	if(HasAuthority())
	{
		int32 id = InputLatentCosmeticEventID;
		if(InputLatentCosmeticEventID < 0)
			id = GetNextLatentCosmeticEventID();
		OutLatentCosmeticEventID = id;
		return InternalTriggerLatentCosmeticEvent(CosmeticEvent, parameters, false, SpawnTransform, id);
	}
	return nullptr;
}

// Called when the game starts or when spawned
void ACosmeticEventManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACosmeticEventManager::MulticastTriggerInstantCosmeticEvent_Implementation(
	TSubclassOf<UInstantCosmeticEvent> CosmeticEvent, const FCosmeticEventData& parameters, const TArray<APlayerController*>& clientsToIgnore)
{
	//clientsToIgnore is coming from server, so if we find a valid player controller in the array, we can assume we should not trigger the event because we are on the ignore list, since the game might be a multiplayer game with several local players on the same machine, we need to check if any of the clients to ignore is a local player controller
	for(const auto playerController : clientsToIgnore)
	{
		if(playerController && playerController->IsValidLowLevel() && playerController->IsLocalPlayerController())
		{
			return;
		}
	}
	
	InternalTriggerInstantCosmeticEvent(CosmeticEvent, parameters);
}

void ACosmeticEventManager::InternalTriggerInstantCosmeticEvent(TSubclassOf<UInstantCosmeticEvent> CosmeticEvent,
                                                                const FCosmeticEventData& parameters)
{
	//Check if subclass is valid
	if(!CosmeticEvent || !CosmeticEvent->IsValidLowLevel())
	{
		UE_LOG(DXCore, Error, TEXT("Tried triggering an InstantCosmeticEvent but the eventClass was invalid."));
		return;
	}

	//Call the Execute from the Default subobject of eventClass passing the parameter
	auto defaultCosmeticEvent = CosmeticEvent->GetDefaultObject<UInstantCosmeticEvent>();
	defaultCosmeticEvent->WorldContext = this->GetWorld();
	defaultCosmeticEvent->Execute(parameters, this->GetWorld());
}

ALatentCosmeticEvent* ACosmeticEventManager::InternalTriggerLatentCosmeticEvent(
	TSubclassOf<ALatentCosmeticEvent> CosmeticEvent, const FCosmeticEventData& parameters, bool bIsLocalOnly, const FTransform& SpawnTransform, int32 LatentCosmeticEventID) const
{
	//Check if subclass is valid
	if(!CosmeticEvent || !CosmeticEvent->IsValidLowLevel())
	{
		UE_LOG(DXCore, Error, TEXT("Tried triggering a LatentCosmeticEvent but the eventClass was invalid."));
		return nullptr;
	}

	if (UWorld* World = GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ALatentCosmeticEvent* SpawnedActor = World->SpawnActorDeferred<ALatentCosmeticEvent>(CosmeticEvent.Get(), SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		// Modify replication property before construction script execution
		SpawnedActor->SetReplicates(!bIsLocalOnly);
		SpawnedActor->EventID = LatentCosmeticEventID;
		SpawnedActor->EventData = parameters;
		SpawnedActor->bServerSpawned = !bIsLocalOnly;
		SpawnedActor->FinishSpawning(SpawnTransform);
		return SpawnedActor;
	}
	else
	{
		UE_LOG(DXCore, Error, TEXT("Tried triggering a LatentCosmeticEvent but the World was invalid."));
		return nullptr;
	}
}

// Called every frame
void ACosmeticEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

