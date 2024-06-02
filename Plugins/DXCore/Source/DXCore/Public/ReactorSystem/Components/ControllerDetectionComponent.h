// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReactorSystem/Reactor.h"
#include "ControllerDetectionComponent.generated.h"

//This Component should be attached to the PlayerController object.
//It's function consists in calling detecting for all overlapping reactors
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DXCORE_API UControllerDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UControllerDetectionComponent();

protected:
	UPROPERTY()
	TArray<AReactor*> reactorsBuffer;
	UPROPERTY()
	TArray<UKeyLocatorComponent*> activeKeyLocators;

	bool bLockedInteract;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Get all overlapping reactors and fire all detect on their detector components*/
	void Detect();

	float GetDistanceToCameraForwardVector(AActor* Actor) const;
	float GetDistanceToPawnForwardVector(AActor* Actor) const;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSortInteractByPawnForwardInsteadOfCamera;

	UFUNCTION(BlueprintPure)
	bool HasInteractables();
	UFUNCTION(BlueprintPure)
	bool HasInteractablesDetectingTrue(FName InteractTag = "");
	UFUNCTION(BlueprintCallable)
	bool Interact(FName InteractTag = "");

	UFUNCTION(BlueprintPure)
	bool IsLockedInteract() const;
	UFUNCTION(BlueprintCallable)
	void LockInteract();
	UFUNCTION(BlueprintCallable)
	void UnlockInteract();

	UFUNCTION(BlueprintPure)
	TArray<UKeyLocatorComponent*> GetActiveLocatorComponents();
	UFUNCTION(BlueprintPure)
	TArray<AReactor*> GetDetectingReactors();
};
