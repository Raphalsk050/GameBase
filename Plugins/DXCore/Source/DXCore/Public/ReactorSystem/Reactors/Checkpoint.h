// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReactorSystem/Reactor.h"

#include "Checkpoint.generated.h"

UCLASS()
class DXCORE_API ACheckpoint : public AReactor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACheckpoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Checkpoint")
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Checkpoint")
	UChildActorComponent* ChildPlayerStart;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Checkpoint")
	FName Identifier;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Checkpoint")
	int32 Order;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Checkpoint")
	uint8 bInitialDev:1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Checkpoint")
	uint8 bInitialBuild:1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;
protected:
	virtual void Activating_Implementation(AController* controller) override;
};
