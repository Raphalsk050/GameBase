// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSystem/DXCoreSaveGame.h"
#include "DXGameSettings.h"
#include "SettingObject.h"
#include "Core/DXCoreGISubsystem.h"

#include "DXGameSettingsSubsystem.generated.h"

struct FSettingConfig;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingChange, TSubclassOf<USettingObject>, SettingChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnySettingChange, bool, AnySettingDirty);
/**
 * 
 */
UCLASS()
class DXGAMESETTINGS_API UDXGameSettingsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable)
	void ChangeSettingValue(TSubclassOf<USettingObject> Setting, FString NewValue);

	UFUNCTION(BlueprintCallable)
	FString GetSettingSavedValue(TSubclassOf<USettingObject> Setting);

	UFUNCTION(BlueprintCallable)
	FString GetSettingCurrentValue(TSubclassOf<USettingObject> Setting);

	FOnSettingChangeValue* GetSettingDelegate(TSubclassOf<USettingObject> Setting);

	UFUNCTION(BlueprintCallable)
	void ApplySettings();
	UFUNCTION(BlueprintCallable)
	void RevertSettings();
	UFUNCTION(BlueprintCallable)
	void SetDefaultSettings();

	UFUNCTION(BlueprintPure)
	bool HasAnyDirtySetting() const;

	UFUNCTION(BlueprintPure)
	USettingObject* GetSettingObjectWithName(const FString& Name) const;
	UFUNCTION(BlueprintPure)
	USettingObject* GetSettingObjectOfClass(TSubclassOf<USettingObject> SettingObjectClass) const;

	UPROPERTY(BlueprintAssignable)
	FOnSettingChange OnSettingChange;

	UPROPERTY(BlueprintAssignable)
	FOnAnySettingChange OnAnySettingsDirty;

protected:
	FMainSaveGameDiskOperation SaveMainSaveGameDiskOperation;
	
private:
	UFUNCTION()
	void ListenMainSaveGameInitializing();

	UFUNCTION()
	void InitializeDefaultSettings();
	
	UFUNCTION()
	void InitializeSavedSettings(bool SuccessLoadSavedData, UDXCoreMainSaveGame* CoreMainSaveGame);

	UFUNCTION()
	void UpdateSettingsSavedValues(const FParameterKeeperData& AlreadySavedSettings);

	UFUNCTION()
	void OnSettingValueChanged(USettingObject* SettingChanged);

	UPROPERTY()
	TArray<USettingObject*> _dirtySettings;
	
	UPROPERTY()
	TArray<USettingObject*> _settingObjects;

};
