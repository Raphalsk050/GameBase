// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buttons/SettingButton.h"
#include "Components/ComboBoxString.h"
#include "SettingDropDownButton.generated.h"

/*
 *		
 */
UCLASS()
class DXCOREUI_API USettingDropDownButton : public USettingButton
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void SetSelectedItemInternal(int32 InValue);
	
	UFUNCTION()
	void ChangeSelectedItem(FString String, ESelectInfo::Type SelectionType);
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UComboBoxString* DropDown_ComboBox;

};
