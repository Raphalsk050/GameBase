// Fill out your copyright notice in the Description page of Project Settings.


#include "CosmeticEventSystem/LatentCosmeticEvent.h"

#include "DXCoreImportantTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ALatentCosmeticEvent::ALatentCosmeticEvent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SetReplicates(true);
	//AActor::SetReplicateMovement(false);
}

void ALatentCosmeticEvent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ALatentCosmeticEvent, EventID, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ALatentCosmeticEvent, EventData, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ALatentCosmeticEvent, bServerSpawned, COND_InitialOnly);
}

// Called when the game starts or when spawned
void ALatentCosmeticEvent::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
	{
		MulticastStartExecute(EventData);
	}

	//This might be a replicated actor that was spawned from the server, so first look for any other LatentCosmeticEvents with the same ID and call PreSpawnReconciliation on myself and the destroy the other one
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALatentCosmeticEvent::StaticClass(), FoundActors);
	for(AActor* Actor : FoundActors)
	{
		ALatentCosmeticEvent* OtherLatentCosmeticEvent = Cast<ALatentCosmeticEvent>(Actor);
		if(OtherLatentCosmeticEvent && OtherLatentCosmeticEvent != this && OtherLatentCosmeticEvent->EventID == EventID)
		{
			if(OtherLatentCosmeticEvent->bStartedExecution)
			{
				PreSpawnReconciliation(OtherLatentCosmeticEvent);
			}
			OtherLatentCosmeticEvent->Destroy();
		}
	}
}

void ALatentCosmeticEvent::PreSpawnReconciliation_Implementation(ALatentCosmeticEvent* OldLocalEvent)
{
}

// Called every frame
void ALatentCosmeticEvent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALatentCosmeticEvent::SetLocalRelevantOnly(bool NewValue)
{
	bLocalRelevantOnly = NewValue;
}

void ALatentCosmeticEvent::ExecutionStart_Implementation(FCosmeticEventData Parameters)
{
}

void ALatentCosmeticEvent::ExecutionStop_Implementation()
{
	Destroy();
}

void ALatentCosmeticEvent::MulticastStartExecute_Implementation(FCosmeticEventData Parameters)
{
	if(!bStartedExecution)
	{
		bStartedExecution = true;
		ExecutionStart(Parameters);
	}
}

void ALatentCosmeticEvent::MulticastStopExecute_Implementation()
{
	if(bStartedExecution)
	{
		bStartedExecution = false;
		ExecutionStop();
	}
}
