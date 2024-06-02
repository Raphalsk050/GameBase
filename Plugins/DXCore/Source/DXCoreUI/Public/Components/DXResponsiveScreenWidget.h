// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DXResponsiveScreenWidget.generated.h"

class USettingObject;
class UDXGameSettingsSubsystem;
/**
 * 
 */
UCLASS()
class DXCOREUI_API UDXResponsiveScreenWidget : public UCommonUserWidget
{
	GENERATED_BODY()

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
protected:
	UFUNCTION()
	void OnSettingChange(TSubclassOf<USettingObject> Setting);
	
	virtual void UpdateSreenSize(bool bFromConstruct);

	FIntVector2 GetDesiredScreenRatio(int32 SettingValue);
	
	UDXGameSettingsSubsystem* GameSettingsSubsystem;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USettingObject> SettingObjectAttached;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	class UDXCustomSizeBox* WidescreenRatio_SizeBox;
	
};
