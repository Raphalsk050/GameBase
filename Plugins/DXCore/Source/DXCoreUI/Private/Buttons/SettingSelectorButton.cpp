#include "Buttons/SettingSelectorButton.h"

void USettingSelectorButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// if (LeftButton) LeftButton->OnClicked.AddDynamic(this, &UCommonRotator::ShiftTextLeft);
	//
	// if (RightButton) RightButton->OnClicked.AddDynamic(this, &UCommonRotator::ShiftTextRight);
}

void USettingSelectorButton::NativeConstruct()
{
	// if (!SettingObject) return;
	//
	// USettingObject* SettingObjAttached = SettingObject.GetDefaultObject();
	//
	// if (!SettingObjAttached) return;
	//
	// TArray<FSettingOption> options = SettingObjAttached->SettingConfig.SettingOptionConfig.SettingOptions;
	// 	
	// for (FSettingOption option : options)
	// {
	// 	TextLabels.Add(option.GetOptionName());
	// }
	//
	// if (TextLabels.Num() <= 0) return;
	//
	// PopulateTextLabels(TextLabels);

	Super::NativeConstruct();
}
