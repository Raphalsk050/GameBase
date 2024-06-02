// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/SettingObject.h"
#include "DXToggleSettingObject.generated.h"

/**
 * A SettingObject that holds a true or false value
 */
UCLASS()
class DXGAMESETTINGS_API UDXToggleSettingObject : public USettingObject
{
	GENERATED_BODY()

public:
	UDXToggleSettingObject(const FObjectInitializer& ObjectInitializer);
	
	virtual void InitSetting_Implementation(UObject* WorldContextObject) override;

	UFUNCTION(BlueprintCallable)
	void ChangeToggleValue(bool NewValue);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnValueChanged(bool NewValue);
	virtual void OnValueChanged_Implementation(bool NewValue);
	
	UFUNCTION(BlueprintPure)
	bool GetToggleValue() const;

private:
	
	UFUNCTION()
	void OnSettingChangeValue_Event(USettingObject* SettingObject);
};
