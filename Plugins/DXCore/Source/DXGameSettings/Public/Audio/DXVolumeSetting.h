// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/DXRangeSettingObject.h"
#include "DXVolumeSetting.generated.h"

/**
 * 
 */
UCLASS()
class DXGAMESETTINGS_API UDXVolumeSetting : public UDXRangeSettingObject
{
	GENERATED_BODY()

public:
	virtual void InitSetting_Implementation(UObject* WorldContextObject) override;
	virtual void OnValueChanged_Implementation(float NormalizedValue) override;

		
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundMix* SoundMix = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundClass* SoundClass = nullptr;
};
