// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DXSelectSettingObject.h"
#include "DXLanguageSetting.generated.h"

/**
 * This class is responsible for changing all the in-game localized UI texts for the desired culture.
 */
UCLASS()
class DXGAMESETTINGS_API UDXLanguageSetting : public UDXSelectSettingObject
{
	GENERATED_BODY()

	UDXLanguageSetting(const FObjectInitializer& ObjectInitializer);
public:
	virtual void ChangeSettingValue_Implementation(const FString& NewValue) override;

	virtual void InitSelectOptions_Implementation() override;
	virtual FText GetTextForOption_Implementation(const FString& OptionStr) override;
};
