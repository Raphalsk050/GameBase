// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DXCoreImportantTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "CosmeticEventSystem/CosmeticEventManager.h"
#include "ReactorSystem/Reactors/Checkpoint.h"
#include "DXCoreWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DXCORE_API UDXCoreWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;

	ACosmeticEventManager* _cosmeticEventManager;

public:

	ACosmeticEventManager* GetCosmeticEventManager();

	FGlobalDataKeeper dataKeeper;
	
	UFUNCTION(BlueprintPure, Category = "Game Mode")
	ELevelPlayState GetLevelPlayState();

	UFUNCTION(BlueprintPure, Category = "Game Mode")
	FName GetCurrentCheckpoint();
	UFUNCTION(BlueprintPure, Category = "Game Mode")
	uint8 GetCurrentCheckpointOrder();
	UFUNCTION(BlueprintPure, Category = "Game Mode")
	ACheckpoint* GetCurrentCheckpointActor();
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
};
