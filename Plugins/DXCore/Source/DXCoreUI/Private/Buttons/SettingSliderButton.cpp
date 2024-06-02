/*
#include "Buttons/SettingSliderButton.h"

void USettingSliderButton::NativeConstruct()
{
	NumericalRotator = true;
	
	if (Slider)
	{
		Slider->SetStepSize(SliderSteps);
		Slider->OnValueChanged.AddDynamic(this, &USettingSliderButton::OnSliderChangeValue);
	}

	if (SettingObject)
	{
		MinIndexRange = SliderMinValue;
		MaxIndexRange = SliderMaxValue;
	}

	Super::NativeConstruct();
	
	OnRotated.AddDynamic(this, &USettingSliderButton::OnButtonRotated);

}

void USettingSliderButton::SetSelectedItemInternal(int32 InValue)
{
	Super::SetSelectedItemInternal(InValue);

	if (!Slider) return;

	float valueMapped = FMath::GetMappedRangeValueClamped(FVector2D(MinIndexRange, MaxIndexRange), FVector2D(0, 1), InValue);

	Slider->SetValue(valueMapped);
}

void USettingSliderButton::OnSliderChangeValue(float NewValue)
{
	int32 valueMapped = FMath::GetMappedRangeValueClamped(FVector2D(0, 1), FVector2D(MinIndexRange, MaxIndexRange), NewValue);

	if (valueMapped == SelectedIndex) return;
	
	SetSelectedItem(valueMapped);
}

void USettingSliderButton::OnButtonRotated(int32 NewValue)
{
	float valueMapped = FMath::GetMappedRangeValueClamped(FVector2D(MinIndexRange, MaxIndexRange), FVector2D(0, 1), NewValue);

	if (Slider)
	{
		Slider->SetValue(valueMapped);
	}
}
*/

