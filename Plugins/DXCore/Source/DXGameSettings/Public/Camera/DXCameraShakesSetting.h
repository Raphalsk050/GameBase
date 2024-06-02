// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/SettingObject.h"
#include "DXCameraShakesSetting.generated.h"

/**
 * 
 */
UCLASS()
class DXGAMESETTINGS_API UDXCameraShakesSetting : public USettingObject
{
	GENERATED_BODY()

public:
	
	virtual void InitSetting_Implementation(UObject* WorldContextObject) override; 
	virtual void ChangeSettingValue_Implementation(const FString& NewValue) override;
};
