// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "Components/TextBlock.h"
#include "DXGameSettings/Public/Core/SettingObject.h"
#include "DXGameSettings/Public/Core/DXGameSettingsSubsystem.h"
#include "SettingButton.generated.h"

struct FSettingConfig; 

/**
 * 
 */
UCLASS(Abstract)
class DXCOREUI_API USettingButton : public UCommonRotator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<USettingObject> SettingObject;

	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	virtual void SetSelectedItem(int32 InValue) override;
	virtual void SetSelectedItemInternal(int32 InValue);
	
	/* Use to update visual of this button with the saved config on DXGameSettingsSubsystem. */
	virtual void UpdateWidgetVisual();

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional))
	UTextBlock* ButtonName_TextBlock;

private:
	UFUNCTION()
	void OnSettingChangeValue(FString NewValue);
	
	UDXGameSettingsSubsystem* _gameSettingsSubsystem;
};
