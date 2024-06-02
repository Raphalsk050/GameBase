// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnalogSlider.h"
#include "SettingButton.h"
#include "Buttons/SettingButton.h"
#include "SettingSliderButton.generated.h"

/**
 * It has a slider that allows you to select a numerical option, it also serves to visually represent the currently selected option.
 */
UCLASS()
class DXCOREUI_API USettingSliderButton  : public USettingButton
{
	GENERATED_BODY()
	
/*public:
	virtual void NativeConstruct() override;
	virtual void SetSelectedItemInternal(int32 InValue) override;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UAnalogSlider* Slider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SliderMinValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SliderMaxValue = 1.0f;

	/** Modifies the slider's StepSize, which defines how much the slider handle moves #1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SliderSteps = 1.0f;

	/** Set SelectedIndex according to slider value #1#
	UFUNCTION()
	void OnSliderChangeValue(float NewValue);

	UFUNCTION()
	void OnButtonRotated(int32 NewValue);*/
};


