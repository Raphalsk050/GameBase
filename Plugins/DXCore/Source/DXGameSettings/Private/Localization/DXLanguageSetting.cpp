// Fill out your copyright notice in the Description page of Project Settings.


#include "Localization/DXLanguageSetting.h"
#include "Kismet/KismetInternationalizationLibrary.h"


UDXLanguageSetting::UDXLanguageSetting(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SettingKey = "Language";
	SettingDefaultValue = "pt-BR";
}

//Updates the current language based on the culture index.
void UDXLanguageSetting::ChangeSettingValue_Implementation(const FString& NewValue)
{
	Super::ChangeSettingValue_Implementation(NewValue);

	if (!SelectOptions.Contains(NewValue))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to change language culture to: %s"), *NewValue);
		return;
	}
		
	UKismetInternationalizationLibrary::SetCurrentCulture(NewValue);
}

void UDXLanguageSetting::InitSelectOptions_Implementation()
{
	Super::InitSelectOptions_Implementation();

	SelectOptions.Append(UKismetInternationalizationLibrary::GetLocalizedCultures());
}

FText UDXLanguageSetting::GetTextForOption_Implementation(const FString& OptionStr)
{
	// TODO: this wasn't working because ComboBox uses strings directly. Make a Text-String relation behind the scenes?
	//return FText::FromString(UKismetInternationalizationLibrary::GetCultureDisplayName(OptionStr, true));

	return FText::FromString(OptionStr);
}
