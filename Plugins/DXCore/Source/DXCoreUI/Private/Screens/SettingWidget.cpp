// Fill out your copyright notice in the Description page of Project Settings.


#include "Screens/SettingWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Buttons/SettingButton.h"
#include "DXCoreUI/Public/Components/ActionBarWidget.h"
#include "DXCoreUI/Public/Screens/ConfirmationPopUpWidget.h"
#include "Input/CommonUIInputTypes.h"


void USettingWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplySettingsInputAction,
		true,
		FSimpleDelegate::CreateUObject(this, &USettingWidget::TryApplySettings)));
	
	RevertHandle = RegisterUIActionBinding(FBindUIActionArgs(RevertSettingsInputAction,
		true,
		FSimpleDelegate::CreateUObject(this, &USettingWidget::TryRevertSettings)));

	SetDefaultHandle = RegisterUIActionBinding(FBindUIActionArgs(SetDefaultSettingsInputAction,
		true,
		FSimpleDelegate::CreateUObject(this, &USettingWidget::TrySetSettingsToDefault)));
	
	GameSettingsSubsystem = GetGameInstance()->GetSubsystem<UDXGameSettingsSubsystem>();
	check(GameSettingsSubsystem);

	GameSettingsSubsystem->OnAnySettingsDirty.AddDynamic(this, &USettingWidget::ToggleUserActions);

	if (SetDefaultSettings_Button)
		SetDefaultSettings_Button->OnConfirmationPopUpCreated.AddDynamic(this, &USettingWidget::ListenConfirmationPopUp);
}

void USettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ToggleUserActions(false);

	if (SetDefaultSettings_Button)
	{
		SetDefaultSettings_Button->SetTriggeredInputAction(SetDefaultSettingsInputAction);
	}

	if (!ActionBarWidget) return;

	ActionBarWidget->SetVisibility(ESlateVisibility::Visible);
	
	if (!GetActionBindings().Contains(SetDefaultHandle))
	{
		AddActionBinding(SetDefaultHandle);
	}
}

void USettingWidget::TryApplySettings()
{
	if (!GameSettingsSubsystem) return;

	GameSettingsSubsystem->ApplySettings();
}

void USettingWidget::TryRevertSettings()
{
	if (!GameSettingsSubsystem) return;

	GameSettingsSubsystem->RevertSettings();
}

void USettingWidget::TrySetSettingsToDefault()
{
	// This is basically a "work around"
	if (SetDefaultSettings_Button)
	{
		SetDefaultSettings_Button->NativeOnClicked();
	}
	else
	{
		SetDefaultSettings();
	}
}

void USettingWidget::SetDefaultSettings()
{
	if (!GameSettingsSubsystem)
		return;

	GameSettingsSubsystem->SetDefaultSettings();
}

void USettingWidget::OnSelectedOption(EOptionType OptionSeleced)
{
	if (OptionSeleced == EOptionType::OT_Confirmed)
	{
		SetDefaultSettings();
	}
}

void USettingWidget::ToggleUserActions(bool bShow)
{
	if (!ActionBarWidget) return;

	ActionBarWidget->SetVisibility(ESlateVisibility::Visible);

	if (bShow)
	{
		if (!GetActionBindings().Contains(ApplyHandle))
		{
			AddActionBinding(ApplyHandle);
		}
		if (!GetActionBindings().Contains(RevertHandle))
		{
			AddActionBinding(RevertHandle);
		}
	}
	else
	{
		RemoveActionBinding(ApplyHandle);
		RemoveActionBinding(RevertHandle);
	}
}

void USettingWidget::ListenConfirmationPopUp(UConfirmationPopUpWidget* PopUp)
{
	if (!PopUp) return;
	
	PopUp->OnOptionSelected.AddDynamic(this, &USettingWidget::OnSelectedOption);
}
