#include "Buttons/SettingDropDownButton.h"

#include "DXCoreFunctionLibrary.h"

void USettingDropDownButton::NativeConstruct()
{
	DropDown_ComboBox->ClearOptions();

	if (!SettingObject) return;

	const auto SettingsSubsystem = GetGameInstance()->GetSubsystem<UDXGameSettingsSubsystem>();

	const auto Setting = SettingsSubsystem->GetSettingObjectOfClass(SettingObject);
	
	if(!Setting) return;
	
	// TArray<FSettingOption> options = Setting->SettingConfig.SettingOptionConfig.SettingOptions;
	//
	// if (options.IsEmpty()) return;
	//
	// for (FSettingOption option : options)
	// {
	// 	TextLabels.Add(option.GetOptionName());
	// }
	//
	// PopulateTextLabels(TextLabels);
	//
	// for (FText optionText : TextLabels )
	// {
	// 	DropDown_ComboBox->AddOption(optionText.ToString());
	// }
	//
	// DropDown_ComboBox->SetSelectedOption(TextLabels[0].ToString());
	// if(!DropDown_ComboBox->OnSelectionChanged.Contains(this, "ChangeSelectedItem"))
	// 	DropDown_ComboBox->OnSelectionChanged.AddDynamic(this, &USettingDropDownButton::ChangeSelectedItem);

	Super::NativeConstruct();
}

void USettingDropDownButton::SetSelectedItemInternal(int32 InValue)
{
	Super::SetSelectedItemInternal(InValue);

	// if(!TextLabels.IsValidIndex(InValue))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Value %d was not valid when selecting an item"), InValue);
	// 	return;
	// }
	// DropDown_ComboBox->SetSelectedOption(TextLabels[InValue].ToString());
}

void USettingDropDownButton::ChangeSelectedItem(FString String, ESelectInfo::Type SelectionType)
{
	// if(SelectionType == ESelectInfo::OnNavigation)
	// 	return;
	//
	// SetSelectedItem(DropDown_ComboBox->GetSelectedIndex());
}
