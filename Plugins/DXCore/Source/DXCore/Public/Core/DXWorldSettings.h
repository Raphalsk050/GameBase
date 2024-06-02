// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DXCoreImportantTypes.h"
#include "GameFramework/WorldSettings.h"
#include "DXWorldSettings.generated.h"

class ACheckpoint;

/**
 * 
 */
UCLASS()
class DXCORE_API ADXWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

private:
	//====== Checkpoints
	FName _currentCheckpoint = "NOCHECKPOINT";
	uint8 _currentCheckpointOrder;
	UPROPERTY()
	AActor* _currentCheckpointActor;
	
protected:
	UPROPERTY(EditAnywhere, Category = "DXCore")
	ELevelPlayState LevelPlayState;

public:
	//Exposed for BP in DXCoreWorldSubsystem instead
	//UFUNCTION(BlueprintPure, Category = "DXCore")
	ELevelPlayState GetLevelPlayState();

	void SetCurrentCheckpoint(ACheckpoint* newCheckpoint);

	FName GetCurrentCheckpoint() const { return _currentCheckpoint; }
	uint8 GetCurrentCheckpointOrder() const { return _currentCheckpointOrder; }
	AActor* GetCurrentCheckpointActor() const { return _currentCheckpointActor; }
};
