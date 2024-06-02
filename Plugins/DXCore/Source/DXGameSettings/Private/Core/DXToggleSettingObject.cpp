// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DXToggleSettingObject.h"

UDXToggleSettingObject::UDXToggleSettingObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SettingType = EDXSettingType::Toggle;
}

void UDXToggleSettingObject::InitSetting_Implementation(UObject* WorldContextObject)
{
	OnSettingChangeValue.AddDynamic(this, &UDXToggleSettingObject::OnSettingChangeValue_Event);
	Super::InitSetting_Implementation(WorldContextObject);
}

void UDXToggleSettingObject::OnSettingChangeValue_Event(USettingObject* SettingObject)
{
	OnValueChanged((bool)GetCurrentValueAsInt());
}

void UDXToggleSettingObject::ChangeToggleValue(bool NewValue)
{
	int intValue = NewValue;
	FString valueString = FString::FromInt(intValue);;
	ChangeSettingValue(valueString);
}

void UDXToggleSettingObject::OnValueChanged_Implementation(bool NewValue)
{
	
}

bool UDXToggleSettingObject::GetToggleValue() const
{
	return (bool)GetCurrentValueAsInt();
}
