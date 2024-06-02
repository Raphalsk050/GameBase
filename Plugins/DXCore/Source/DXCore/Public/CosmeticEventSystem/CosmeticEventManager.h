// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InstantCosmeticEvent.h"
#include "LatentCosmeticEvent.h"
#include "GameFramework/Actor.h"
#include "CosmeticEventManager.generated.h"

UCLASS()
class DXCORE_API ACosmeticEventManager : public AActor
{
	GENERATED_BODY()

private:
	int32 _nextLatentCosmeticEventID = 0;
protected:
	int32 GetNextLatentCosmeticEventID()
	{
		return _nextLatentCosmeticEventID = (++_nextLatentCosmeticEventID == MAX_int32) ? 1 : _nextLatentCosmeticEventID;
	}
public:
	// Sets default values for this actor's properties
	ACosmeticEventManager();

	
	//Auth and Local version of the TriggerInstantCosmeticEvent function
	UFUNCTION(Category="Cosmetic Event")
	void LocalTriggerInstantCosmeticEvent(TSubclassOf<UInstantCosmeticEvent> CosmeticEvent, const FCosmeticEventData& parameters);

	UFUNCTION(Category="Cosmetic Event")
	void AuthTriggerInstantCosmeticEvent(TSubclassOf<UInstantCosmeticEvent> CosmeticEvent, const FCosmeticEventData& parameters, const TArray<APlayerController*>& clientsToIgnore);

	UFUNCTION(Category="Cosmetic Event")
	ALatentCosmeticEvent* LocalTriggerLatentCosmeticEvent(TSubclassOf<ALatentCosmeticEvent> CosmeticEvent, const FCosmeticEventData& parameters, FTransform SpawnTransform, int32& OutLatentCosmeticEventID);

	UFUNCTION(Category="Cosmetic Event")
	ALatentCosmeticEvent* AuthTriggerLatentCosmeticEvent(TSubclassOf<ALatentCosmeticEvent> CosmeticEvent, const FCosmeticEventData& parameters, const FTransform& SpawnTransform, int32& OutLatentCosmeticEventID, int32 InputLatentCosmeticEventID = -1);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Unreliable, Category="Cosmetic Event")
	void MulticastTriggerInstantCosmeticEvent(TSubclassOf<UInstantCosmeticEvent> CosmeticEvent, const FCosmeticEventData& parameters, const TArray<APlayerController*>& clientsToIgnore);
	
	void InternalTriggerInstantCosmeticEvent(TSubclassOf<UInstantCosmeticEvent> CosmeticEvent, const FCosmeticEventData& parameters);

	ALatentCosmeticEvent* InternalTriggerLatentCosmeticEvent(TSubclassOf<ALatentCosmeticEvent> CosmeticEvent, const FCosmeticEventData& parameters, bool bIsLocalOnly, const FTransform& SpawnTransform, int32 LatentCosmeticEventID) const;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
