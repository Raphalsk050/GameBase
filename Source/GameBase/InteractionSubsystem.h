// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystem/Public/InteractionKey.h"
#include "Subsystems/WorldSubsystem.h"
#include "InteractionSubsystem.generated.h"

/**
 * 
 */

UINTERFACE(BlueprintType)
class GAMEBASE_API UObserver : public UInterface
{
	GENERATED_BODY()
};

class IObserver
{
	GENERATED_BODY()

public:
	virtual void OnNotify(FString Key) = 0;
};

UCLASS()
class GAMEBASE_API UInteractionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
    
private:
	UPROPERTY()
	TArray<FInteractionKey> InteractionKeys;
	
	
	TMap<FString, TArray<IObserver*>> Observers;

public:
	// Initialize the subsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	// Deinitialize the subsystem
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	bool RegisterKey(const FString KeyName, const bool InitialValue, const EDetectionType DetectionType);

	UFUNCTION(BlueprintCallable)
	void SendGlobalRequirement(const FString KeyName, const bool Value);
	
	UFUNCTION(BlueprintCallable)
	void UnregisterKey(FString Key);
	
	UFUNCTION(BlueprintCallable)
	void CheckAndTriggerKeys();
	
	void RegisterObserver(const FString KeyName, const bool InitialValue, const EDetectionType DetectionType, IObserver* Observer);
	void UnregisterObserver(FString Key, IObserver* Observer);
	void NotifyObservers(FString Key);
};
