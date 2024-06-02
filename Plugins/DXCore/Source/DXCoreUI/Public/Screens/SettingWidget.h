// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActionBarWidget.h"
#include "Buttons/BaseButton.h"
#include "CommonActivatableWidget.h"
#include "DXGameSettings/Public/Core/DXGameSettingsSubsystem.h"
#include "SettingWidget.generated.h"

enum class EOptionType : uint8;
class USettingButton;

UCLASS()
class DXCOREUI_API USettingWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UDXGameSettingsSubsystem* GameSettingsSubsystem;
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category = "Core Settings")
	virtual void TryApplySettings();

	UFUNCTION(BlueprintCallable, Category = "Core Settings")
	virtual void TryRevertSettings();

	UFUNCTION(BlueprintCallable, Category = "Core Settings")
	virtual void TrySetSettingsToDefault();
	
	UFUNCTION(BlueprintCallable, Category = "Core Settings")
	virtual void SetDefaultSettings();

	UFUNCTION()
	virtual void OnSelectedOption(EOptionType OptionSeleced);

	UPROPERTY(EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase))
	FDataTableRowHandle ApplySettingsInputAction;

	UPROPERTY(EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase))
	FDataTableRowHandle RevertSettingsInputAction;

	UPROPERTY(EditDefaultsOnly, meta = (RowType = CommonInputActionDataBase))
	FDataTableRowHandle SetDefaultSettingsInputAction;

	FUIActionBindingHandle ApplyHandle;
	FUIActionBindingHandle RevertHandle;
	FUIActionBindingHandle SetDefaultHandle;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UActionBarWidget* ActionBarWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBaseButton* SetDefaultSettings_Button;

private:
	UFUNCTION()
	virtual void ToggleUserActions(bool bShow);

	UFUNCTION()
	void ListenConfirmationPopUp(UConfirmationPopUpWidget* PopUp); 
};
