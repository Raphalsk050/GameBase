#include "Buttons/SettingButton.h"

void USettingButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	// if (ButtonName_TextBlock && SettingObject)
	// {
	// 	ButtonName_TextBlock->SetText(SettingObject.GetDefaultObject()->SettingConfig.SettingDisplayConfig.SettingDisplayName);
	// }
}

void USettingButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_gameSettingsSubsystem = GetGameInstance()->GetSubsystem<UDXGameSettingsSubsystem>();
	check(_gameSettingsSubsystem);

	if (!SettingObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid or NULL SettingObject attached."));
		return;
	}

	// FOnSettingChangeValue* onSettingChangeValue = _gameSettingsSubsystem->GetSettingDelegate(SettingObject);
	//
	// if (!onSettingChangeValue)
	// 	return;
	//
	// onSettingChangeValue->AddDynamic(this, &USettingButton::OnSettingChangeValue);
}

void USettingButton::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateWidgetVisual();

	// if (ButtonName_TextBlock && SettingObject)
	// {
	// 	ButtonName_TextBlock->SetText(SettingObject.GetDefaultObject()->SettingConfig.SettingDisplayConfig.SettingDisplayName);
	// }
}

void USettingButton::SetSelectedItem(int32 InValue)
{
	Super::SetSelectedItem(InValue);

	// check(_gameSettingsSubsystem);
	//
	// _gameSettingsSubsystem->ChangeSettingValue(SettingObject, InValue);
}

void USettingButton::SetSelectedItemInternal(int32 InValue)
{
	Super::SetSelectedItem(InValue);
}

void USettingButton::UpdateWidgetVisual()
{
	check(_gameSettingsSubsystem);

	// int savedValue = _gameSettingsSubsystem->GetSettingSavedValue(SettingObject);
	//
	// SetSelectedItemInternal(savedValue);
}

void USettingButton::OnSettingChangeValue(FString NewValue)
{
	// if (NewValue == SelectedIndex)
	// 	return;
	//
	// SetSelectedItemInternal(NewValue);
}

