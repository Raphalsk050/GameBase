// Fill out your copyright notice in the Description page of Project Settings.


#include "CosmeticEventSystem/CosmeticEventsComponent.h"

#include "DXCore.h"
#include "DXCoreFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UCosmeticEventsComponent::UCosmeticEventsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCosmeticEventsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UCosmeticEventsComponent::ServerTravelInstantEvent_Implementation(TSubclassOf<UInstantCosmeticEvent> eventClass,
	const FCosmeticEventData& parameters, APlayerController* ownerPlayerController)
{
	AActor* Owner = GetOwner();
	if(Owner->HasAuthority())
	{
		TArray<APlayerController*> clientsToIgnore;
		if(ownerPlayerController)
		{
			clientsToIgnore.Add(ownerPlayerController);
		}
		UDXCoreFunctionLibrary::AuthTriggerInstantCosmeticEvent(eventClass, parameters, clientsToIgnore, GetOwner());
	}
}


void UCosmeticEventsComponent::ServerTravelLatentEvent_Implementation(TSubclassOf<ALatentCosmeticEvent> eventClass,
	const FCosmeticEventData& parameters, FTransform SpawnTransform, int32 localPredictionEventId)
{
	AActor* Owner = GetOwner();
	if(Owner->HasAuthority())
	{
		int32 unusedOutLatentCosmeticEventID;
		UDXCoreFunctionLibrary::AuthTriggerLatentCosmeticEvent(eventClass, parameters, SpawnTransform,GetOwner(), unusedOutLatentCosmeticEventID, localPredictionEventId);
	}
}

void UCosmeticEventsComponent::ServerTravelStopLatentEvent_Implementation(int32 LatentCosmeticEventID)
{
	StopLatentCosmeticEvent(LatentCosmeticEventID);
}

// Called every frame
void UCosmeticEventsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCosmeticEventsComponent::TriggerInstantCosmeticEvent(TSubclassOf<UInstantCosmeticEvent> eventClass,
	const FCosmeticEventData& parameters, bool bLocalOnly)
{
	if(bLocalOnly)
	{
		if(GetNetMode() == NM_DedicatedServer)
			return;
		
		UDXCoreFunctionLibrary::LocalTriggerInstantCosmeticEvent(eventClass, parameters, GetOwner());
		return;
	}
	if(GetNetMode() < NM_Client)
	{
		TArray<APlayerController*> clientsToIgnore;
		UDXCoreFunctionLibrary::AuthTriggerInstantCosmeticEvent(eventClass, parameters, clientsToIgnore, GetOwner());
	}
	else
	{
		if(GetOwner()->HasLocalNetOwner())
		{
			//Get first player controller from game instance
			const UGameInstance* gameInstance = GetWorld()->GetGameInstance();
			const ULocalPlayer* localPlayer = gameInstance->GetFirstGamePlayer();
			if(APlayerController* playerController = localPlayer->GetPlayerController(GetOwner()->GetWorld()))
			{
				UDXCoreFunctionLibrary::LocalTriggerInstantCosmeticEvent(eventClass, parameters, GetOwner());
				ServerTravelInstantEvent(eventClass, parameters, playerController);
			}
		}
		else
		{
			//UDXCoreFunctionLibrary::LocalTriggerInstantCosmeticEvent(eventClass, parameters, GetOwner());
			UE_LOG(DXCore, Error, TEXT("Instant Cosmetic event being called with bLocalOnly as false from client in actor that is not net owned"));
		}
	}	
}

void UCosmeticEventsComponent::TriggerLatentCosmeticEvent(TSubclassOf<ALatentCosmeticEvent> eventClass,
	const FCosmeticEventData& parameters, FTransform SpawnTransform, bool bLocalOnly, int32& OutLatentCosmeticEventID)
{
	int32 LocalEventId;
	if(bLocalOnly)
	{
		if(GetNetMode() == NM_DedicatedServer)
			return;
		
		auto localOnlyEvent = UDXCoreFunctionLibrary::LocalTriggerLatentCosmeticEvent(eventClass, parameters, SpawnTransform, GetOwner(), LocalEventId);
		OutLatentCosmeticEventID = LocalEventId;
		return;
	}
	if(GetNetMode() < NM_Client)
	{
		UDXCoreFunctionLibrary::AuthTriggerLatentCosmeticEvent(eventClass, parameters, SpawnTransform, GetOwner(), OutLatentCosmeticEventID, -1);
		return;
	}
	else
	{
		if(GetOwner()->HasLocalNetOwner())
		{
			auto localPredictionEvent = UDXCoreFunctionLibrary::LocalTriggerLatentCosmeticEvent(eventClass, parameters, SpawnTransform, GetOwner(), LocalEventId);
			OutLatentCosmeticEventID = LocalEventId;
			ServerTravelLatentEvent(eventClass, parameters, SpawnTransform, LocalEventId);
			return;
		}
		else
		{
			//auto localPredictionEvent = UDXCoreFunctionLibrary::LocalTriggerLatentCosmeticEvent(eventClass, parameters, SpawnTransform, GetOwner(), LocalEventId);
			//OutLatentCosmeticEventID = LocalEventId;
			UE_LOG(DXCore, Error, TEXT("Latent cosmetic event being called with bLocalOnly as false from client in actor that is not net owned"));
			return;
		}
	}
	return;
}

void UCosmeticEventsComponent::StopLatentCosmeticEvent(int32 LatentCosmeticEventID)
{
	//Find Latent Cosmetic Event actors with the same ID and stop them
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALatentCosmeticEvent::StaticClass(), FoundActors);
	for(AActor* actor : FoundActors)
	{
		ALatentCosmeticEvent* latentCosmeticEvent = Cast<ALatentCosmeticEvent>(actor);
		if(latentCosmeticEvent && latentCosmeticEvent->EventID == LatentCosmeticEventID)
		{
			if(latentCosmeticEvent->bServerSpawned)
			{
				if(GetNetMode() < NM_Client)
				{
					latentCosmeticEvent->MulticastStopExecute();
				}
				else
				{
					if(GetOwner()->HasLocalNetOwner())
					{
						ServerTravelStopLatentEvent(LatentCosmeticEventID);
					}
					else
					{
						UE_LOG(DXCore, Error, TEXT("Tried stopping a server spawned latent cosmetic event from client code, even will stop local only"));
						//latentCosmeticEvent->MulticastStopExecute();
					}
					
				}
			}
			else
			{
				latentCosmeticEvent->MulticastStopExecute();
			}
		}
	}	
}

ALatentCosmeticEvent* UCosmeticEventsComponent::GetLatentCosmeticEventFromID(int32 LatentCosmeticEventID) const
{
	return UDXCoreFunctionLibrary::GetLatentCosmeticEventFromID(LatentCosmeticEventID, GetOwner());
}

