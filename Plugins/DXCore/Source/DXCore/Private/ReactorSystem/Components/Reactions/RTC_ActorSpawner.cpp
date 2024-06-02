// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorSystem/Components/Reactions/RTC_ActorSpawner.h"

URTC_ActorSpawner::URTC_ActorSpawner()
{
	
}

void URTC_ActorSpawner::SpawnActor()
{
	if(SpawnLocationTarget->IsValidLowLevelFast())
	{
		FVector _newLocation = SpawnLocationTarget->GetActorLocation();
		FRotator _newRotation = SpawnLocationTarget->GetActorRotation();
		FActorSpawnParameters _spawnInfo;
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, _newLocation, _newRotation, _spawnInfo);
		
		if (Attached)
			SpawnedActor->AttachToActor(GetOwner(),FAttachmentTransformRules::KeepWorldTransform);
	}
	else
	{
		FVector _newLocation = GetOwner()->GetActorLocation();
		FRotator _newRotation = GetOwner()->GetActorRotation();
		FActorSpawnParameters _spawnInfo;
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, _newLocation, _newRotation, _spawnInfo);
		
		if (Attached)
			SpawnedActor->AttachToActor(GetOwner(),FAttachmentTransformRules::KeepWorldTransform);
	}
}

void URTC_ActorSpawner::OnReactorActivating_Implementation(AController* controller)
{
	Super::OnReactorActivating_Implementation(controller);
	SpawnActor();
}