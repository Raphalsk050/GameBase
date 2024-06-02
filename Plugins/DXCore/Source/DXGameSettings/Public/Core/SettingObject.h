// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SettingObject.generated.h"

UENUM(BlueprintType)
enum class EDXSettingType : uint8
{
	None,
	Toggle, // On/Off
	Select, // Chose one from multiple choices
	Range   // Pick a value from a float range
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingChangeValue, class USettingObject*, SettingObject);

/**
 *  An object that represents a given setting.
 *  DXGameSettingsSubsystem creates and manages every USettingObject in GameSettingObjectClasses config.
 *  Settings names and values are stored as FStrings so as to store it inside a parameter keeper.
 *  Supports syncing with GameUserSettings to immediately load previous configurations before active user is set (before first input). 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class DXGAMESETTINGS_API USettingObject : public UObject
{
	GENERATED_BODY()
	
public:

	// A unique key to identify this setting inside the save game parameter keeper
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString SettingKey;
	// User readable text to serve as this setting's title or name
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText SettingTitle;
	// User readable text to describe this setting's functionality (optional)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText SettingDescription;
	// Default value. Override 'GetSettingInitialValue' to provide custom logic for the default value.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString SettingDefaultValue;
	
	UPROPERTY(BlueprintAssignable)
	FOnSettingChangeValue OnSettingChangeValue;

	UFUNCTION(BlueprintNativeEvent)
	void InitSetting(UObject* WorldContextObject);
	virtual void InitSetting_Implementation(UObject* WorldContextObject);

	UFUNCTION(BlueprintNativeEvent)
	void ChangeSettingValue(const FString& NewValue);
	virtual void ChangeSettingValue_Implementation(const FString& NewValue);

	// Returns the initial value for the setting
	// By default, returns 'SettingDefaultValue' but can be overriden
	UFUNCTION(BlueprintNativeEvent)
	FString GetSettingInitialValue();
	virtual FString GetSettingInitialValue_Implementation();

	UFUNCTION(BlueprintPure)
	FString GetSettingCurrentValue() const;

	UFUNCTION(BlueprintPure)
	FString GetSettingSavedValue() const;

	UFUNCTION(BlueprintPure)
	bool IsDirty() const;

	void RevertSetting();
	void RevertSettingToDefault();

	int GetCurrentValueAsInt() const { return FCString::Atoi(ToCStr(CurrentValue)); }
	float GetCurrentValueAsFloat() const { return FCString::Atof(ToCStr(CurrentValue)); }
	
	void SetSavedValue(const FString& NewSavedValue);

	// Use to set saved value onto game user settings if necessary
	UFUNCTION(BlueprintNativeEvent)
	void SyncSavedWithGameUserSettings(UGameUserSettings* GameUserSettings);
	virtual void SyncSavedWithGameUserSettings_Implementation(UGameUserSettings* GameUserSettings);

	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintPure)
	EDXSettingType GetSettingType() const { return SettingType; }

protected:
	FString CurrentValue;
	FString SettingSavedValue;

	UPROPERTY(EditAnywhere)
	EDXSettingType SettingType;

	UPROPERTY()
	UObject* SettingsSubsystem;
};

