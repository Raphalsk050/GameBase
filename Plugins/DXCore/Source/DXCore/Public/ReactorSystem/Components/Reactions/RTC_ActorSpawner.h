// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReactorSystem/Components/ReactionComponent.h"
#include "RTC_ActorSpawner.generated.h"

//Spawns an Actor using the owning Reactor's location and rotation.
UCLASS(ClassGroup = (Reaction), meta = (BlueprintSpawnableComponent))
class DXCORE_API URTC_ActorSpawner : public UReactionComponent
{
	GENERATED_BODY()

	URTC_ActorSpawner();

public:
	
	UPROPERTY(EditAnywhere, Category = "Properties")
	UClass* ActorToSpawn;

	//Should the spawned actor be attached to the Reactor after spawned?
	UPROPERTY(EditAnywhere, Category = "Properties")
	bool Attached = true;
	
	//If set, the actor will spawn using this actor's location and rotation instead of its Reactor's.
	UPROPERTY(EditAnywhere, Category = "Properties")
	AActor* SpawnLocationTarget;
	
protected:

	UFUNCTION(BlueprintCallable, Category = "Reaction")
	virtual void OnReactorActivating_Implementation(AController* controller) override;

	virtual void SpawnActor();
};
