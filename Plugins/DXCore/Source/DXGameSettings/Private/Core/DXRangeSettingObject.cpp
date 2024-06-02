// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DXRangeSettingObject.h"

UDXRangeSettingObject::UDXRangeSettingObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SettingType = EDXSettingType::Range;
}

void UDXRangeSettingObject::InitSetting_Implementation(UObject* WorldContextObject)
{
	OnSettingChangeValue.AddDynamic(this, &UDXRangeSettingObject::OnSettingChangeValue_Event);
	Super::InitSetting_Implementation(WorldContextObject);
}

void UDXRangeSettingObject::OnSettingChangeValue_Event(USettingObject* SettingObject)
{
	OnValueChanged(GetCurrentValueAsFloat());
}

void UDXRangeSettingObject::ChangeRangeValue(float NewValue)
{
	float value = FMath::GetMappedRangeValueClamped(FVector2f{0.f, 1.f}, FVector2f{MinRangeValue, MaxRangeValue}, NewValue);
	FString valueString = FString::SanitizeFloat(value);
	ChangeSettingValue(valueString);
}

void UDXRangeSettingObject::OnValueChanged_Implementation(float NewValue)
{
}

float UDXRangeSettingObject::GetRangeValue() const
{
	return GetCurrentValueAsFloat();
}
