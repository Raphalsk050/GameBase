// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DXSelectSettingObject.h"

UDXSelectSettingObject::UDXSelectSettingObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SettingType = EDXSettingType::Select;
}

void UDXSelectSettingObject::InitSetting_Implementation(UObject* WorldContextObject)
{
	OnSettingChangeValue.AddDynamic(this, &UDXSelectSettingObject::OnSettingChangeValue_Event);
	Super::InitSetting_Implementation(WorldContextObject);
	InitSelectOptions();
	InitTextOptions();
}

void UDXSelectSettingObject::OnSettingChangeValue_Event(USettingObject* SettingObject)
{
	OnValueChanged(GetSettingCurrentValue());
}

void UDXSelectSettingObject::ChangeSelectIndex(int Index)
{
	if (!SelectOptions.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Error, TEXT("UDXSelectSettingObject::ChangeSelectValue tried to access invalid index %i!"), Index);
		return;
	}
		
	ChangeSettingValue(SelectOptions[Index]);
}

void UDXSelectSettingObject::ChangeSelectOption(const FString& Option)
{
	int idx = SelectOptions.Find(Option);
	if (idx == INDEX_NONE)
	{
		UE_LOG(LogTemp, Error, TEXT("UDXSelectSettingObject::ChangeSelectValue Option %s isn't in the SelectOptions!"), *Option);
		return;
	}

	ChangeSettingValue(SelectOptions[idx]);
}

void UDXSelectSettingObject::OnValueChanged_Implementation(const FString& NewValue)
{
}

FText UDXSelectSettingObject::GetTextForOption_Implementation(const FString& OptionStr)
{
	return FText::FromString(OptionStr);
}

void UDXSelectSettingObject::InitSelectOptions_Implementation()
{
}

void UDXSelectSettingObject::InitTextOptions()
{
	SelectOptionsTexts.Empty();
	for (const FString& option : SelectOptions)
	{
		SelectOptionsTexts.Add(GetTextForOption(option));
	}
}

int UDXSelectSettingObject::GetCurrentSelectIndex() const
{
	int index = SelectOptions.Find(GetSettingCurrentValue());
	return index;
}
