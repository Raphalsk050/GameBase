// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/SettingObject.h"
#include "DXRangeSettingObject.generated.h"

/**
 * A SettingObject that holds a numerical value inside a given range [min, max]
 */
UCLASS()
class DXGAMESETTINGS_API UDXRangeSettingObject : public USettingObject
{
	GENERATED_BODY()

public:
	UDXRangeSettingObject(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinRangeValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxRangeValue = 1.f;

	virtual void InitSetting_Implementation(UObject* WorldContextObject) override;

	UFUNCTION(BlueprintCallable)
	void ChangeRangeValue(float NewValue);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnValueChanged(float NewValue);
	virtual void OnValueChanged_Implementation(float NewValue);

	UFUNCTION(BlueprintPure)
	float GetRangeValue() const;

private:
	UFUNCTION()
	void OnSettingChangeValue_Event(USettingObject* SettingObject);
};