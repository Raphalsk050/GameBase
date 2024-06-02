// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DXCoreImportantTypes.h"
#include "UObject/Interface.h"
#include "SaveStateInterface.generated.h"

#define GENERATE_SAVESTATE_BODY() \
	GENERATE_SAVESTATE_BODY_CustomSaveLoad() \
	virtual void SaveInitialState_Implementation() override { Execute_StoreState(this, Execute_GetInitialStateKeeper(this), ESaveStateStoreOperation::SSSO_SaveInitialState); } \
	virtual void SaveState_Implementation() override { Execute_StoreState(this, Execute_GetSavedStateKeeper(this), ESaveStateStoreOperation::SSSO_SaveState); } \
	virtual void RestartToInitialState_Implementation() override { Execute_SetupByParameters(this, Execute_GetInitialStateKeeper(this), ESaveStateSetupOperation::SSSO_Restarting); } \
	virtual void LoadState_Implementation() override { Execute_SetupByParameters(this, Execute_GetSavedStateKeeper(this), ESaveStateSetupOperation::SSSO_Loading); }

#define GENERATE_SAVESTATE_BODY_CustomSaveLoad() \
	GENERATE_SAVESTATE_BODY_CustomAllFunctions() \
	virtual void ImportSavedState_Implementation(FParameterKeeperData importData) override { Execute_GetSavedStateKeeper(this)->ImportData(importData); } \
	virtual FParameterKeeperData ExportSavedStateData_Implementation() override { return Execute_GetSavedStateKeeper(this)->ExportParameterData(); } \
	virtual void OverrideNextSavedState_Implementation(FParameterKeeperData overrideData) override { Execute_GetSavedStateKeeper(this)->OverrideData(overrideData); }

#define GENERATE_SAVESTATE_BODY_CustomAllFunctions() \
	const FName InitialStateKeeperName = FName("InitialState"); \
	const FName SavedStateKeeperName = FName("SavedState"); \
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Default|SaveState") \
	UParameterKeeper* InitialState; \
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Default|SaveState") \
	UParameterKeeper* SavedState; \
	virtual UParameterKeeper* GetInitialStateKeeper_Implementation() override { return InitialState; } \
	virtual UParameterKeeper* GetSavedStateKeeper_Implementation() override { return SavedState; }


#define SAVESTATE_INIT() \
	InitialState = CreateDefaultSubobject<UParameterKeeper>(InitialStateKeeperName); \
	check(InitialState); \
	SavedState = CreateDefaultSubobject<UParameterKeeper>(SavedStateKeeperName); \
	check(SavedState);

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class DXCORE_API USaveStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DXCORE_API ISaveStateInterface
{
	GENERATED_BODY()

	//This interface was planned for each class to have their own UParameterKeeper for InitialState and SavedState
	//and provide access to them through this interface.

protected:
	//----- Basic Accessors to help boilerplate code
	
	//Get the actual parameter keeper instance for InitialState, mostly for boilerplate code
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveState")
	UParameterKeeper* GetInitialStateKeeper();
	virtual UParameterKeeper* GetInitialStateKeeper_Implementation();
	//Get the actual parameter keeper instance for SavedState, mostly for boilerplate code
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveState")
	UParameterKeeper* GetSavedStateKeeper();
	virtual UParameterKeeper* GetSavedStateKeeper_Implementation();
	
	//---- Main functionality to be overridden

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveState")
	void StoreState(UParameterKeeper* ParameterKeeper, ESaveStateStoreOperation Operation);
	virtual void StoreState_Implementation(UParameterKeeper* ParameterKeeper, ESaveStateStoreOperation Operation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveState")
	void SetupByParameters(UParameterKeeper* ParameterKeeper, ESaveStateSetupOperation Operation);
	virtual void SetupByParameters_Implementation(UParameterKeeper* ParameterKeeper, ESaveStateSetupOperation Operation);

public:
	//---- Mainly usage functions (probably shouldn't override them)

	//Function that ends up calling StoreState for gathering initial state into the InitialState ParameterKeeper, you need call this yourself in each actor.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveState")
	void SaveInitialState();
	virtual void SaveInitialState_Implementation();
	
	//Function that calls StoreState for gathering state from current simulation into the SavedState ParameterKeeper, called by Checkpoints
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveState")
	void SaveState();
	virtual void SaveState_Implementation();
	
	//Restart (Applies) back to local Initial State (calls SetupByParameters with InitialState as input)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveState")
	void RestartToInitialState();
	virtual void RestartToInitialState_Implementation();

	//Loads (Applies) the local Saved State (calls SetupByParameters with SavedState as input)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveState")
	void LoadState();
	virtual void LoadState_Implementation();

	//A function to put the SavedState data from GameSave file into the local SavedState UParameterKeeper
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveState")
	void ImportSavedState(FParameterKeeperData importData);
	virtual void ImportSavedState_Implementation(FParameterKeeperData importData);

	//Get current data from current SavedState UParameterKeeper
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveState")
	FParameterKeeperData ExportSavedStateData();
	virtual FParameterKeeperData ExportSavedStateData_Implementation();

	//Override Saved State just until the override is consumed. Useful for deterministic loading (used before LoadState)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SaveState")
	void OverrideNextSavedState(FParameterKeeperData overrideData);
	virtual void OverrideNextSavedState_Implementation(FParameterKeeperData overrideData);
};
