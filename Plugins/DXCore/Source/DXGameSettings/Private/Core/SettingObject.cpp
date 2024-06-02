// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SettingObject.h"

void USettingObject::InitSetting_Implementation(UObject* WorldContextObject)
{
	SettingsSubsystem = WorldContextObject;
	SettingSavedValue = GetSettingInitialValue();
	ChangeSettingValue(SettingSavedValue);
}

void USettingObject::ChangeSettingValue_Implementation(const FString& NewValue)
{
	CurrentValue = NewValue;
	OnSettingChangeValue.Broadcast(this);
}

FString USettingObject::GetSettingInitialValue_Implementation()
{
	return SettingDefaultValue;
}

FString USettingObject::GetSettingCurrentValue() const
{
	return CurrentValue;
}

FString USettingObject::GetSettingSavedValue() const
{
	return SettingSavedValue;
}

bool USettingObject::IsDirty() const
{
	return CurrentValue != SettingSavedValue;
}

void USettingObject::RevertSetting()
{
	ChangeSettingValue(SettingSavedValue);
}

void USettingObject::RevertSettingToDefault()
{
	ChangeSettingValue(SettingDefaultValue);
}

void USettingObject::SetSavedValue(const FString& NewSavedValue)
{
	SettingSavedValue = NewSavedValue;
	OnSettingChangeValue.Broadcast(this);
}

void USettingObject::SyncSavedWithGameUserSettings_Implementation(UGameUserSettings* GameUserSettings)
{
}

UWorld* USettingObject::GetWorld() const
{
	if (!SettingsSubsystem)
		return nullptr;

	return SettingsSubsystem->GetWorld();
}
