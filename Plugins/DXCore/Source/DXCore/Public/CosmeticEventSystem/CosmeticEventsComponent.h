// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CosmeticEventsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, DisplayName="Cosmetic Events Component"))
class DXCORE_API UCosmeticEventsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCosmeticEventsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(Server, Unreliable, Category="Cosmetic Event System")
	void ServerTravelInstantEvent(TSubclassOf<UInstantCosmeticEvent> eventClass, const FCosmeticEventData& parameters, APlayerController* ownerPlayerController);

	//ServerTravel for latent event
	UFUNCTION(Server, Unreliable, Category="Cosmetic Event System")
	void ServerTravelLatentEvent(TSubclassOf<ALatentCosmeticEvent> eventClass, const FCosmeticEventData& parameters, FTransform SpawnTransform, int32 localPredictionEventId);

	UFUNCTION(Server, Reliable, Category="Cosmetic Event System")
	void ServerTravelStopLatentEvent(int32 LatentCosmeticEventID);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Cosmetic Event System")
	void TriggerInstantCosmeticEvent(TSubclassOf<UInstantCosmeticEvent> eventClass, const FCosmeticEventData& parameters, bool bLocalOnly);

	UFUNCTION(BlueprintCallable, Category="Cosmetic Event System")
	void TriggerLatentCosmeticEvent(TSubclassOf<ALatentCosmeticEvent> eventClass, const FCosmeticEventData& parameters, FTransform SpawnTransform, bool bLocalOnly, int32& OutLatentCosmeticEventID);

	UFUNCTION(BlueprintCallable, Category="Cosmetic Event System")
	void StopLatentCosmeticEvent(int32 LatentCosmeticEventID);

	UFUNCTION(BlueprintPure, Category = "Cosmetic Event System", meta = (ToolTip = "Beware! The reference is ephemeral and should not be cached / stored, e.g. local versions could be destroyed for server reconciliation when replicated version arrives. Instead always use the EventID in this getter to get the event reference.", ShortTip = "Get the event reference from the EventID, ephemeral reference."))
	ALatentCosmeticEvent* GetLatentCosmeticEventFromID(int32 LatentCosmeticEventID) const;
};
