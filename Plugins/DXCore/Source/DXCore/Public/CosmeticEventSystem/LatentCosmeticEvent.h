// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DXCoreImportantTypes.h"
#include "GameFramework/Actor.h"
#include "LatentCosmeticEvent.generated.h"

UCLASS(Blueprintable)
class DXCORE_API ALatentCosmeticEvent : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALatentCosmeticEvent();
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	int32 EventID;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	FCosmeticEventData EventData;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	bool bServerSpawned;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool bStartedExecution = false;
	bool bLocalRelevantOnly = false;

	UFUNCTION(BlueprintNativeEvent, Category="Cosmetic Event")
	void ExecutionStart(FCosmeticEventData Parameters);
	virtual void ExecutionStart_Implementation(FCosmeticEventData Parameters);

	UFUNCTION(BlueprintNativeEvent, Category="Cosmetic Event")
	void ExecutionStop();
	virtual void ExecutionStop_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category="Cosmetic Event")
	void PreSpawnReconciliation(ALatentCosmeticEvent* OldLocalEvent);
	virtual void PreSpawnReconciliation_Implementation(ALatentCosmeticEvent* OldLocalEvent);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetLocalRelevantOnly(bool NewValue);

	UFUNCTION(NetMulticast, Unreliable, Category="Cosmetic Event")
	void MulticastStartExecute(FCosmeticEventData Parameters);

	//Multicast StopExecute
	UFUNCTION(NetMulticast, BlueprintCallable, Reliable, Category="Cosmetic Event", meta=(DisplayName="Stop Execute"))
	void MulticastStopExecute();
};
