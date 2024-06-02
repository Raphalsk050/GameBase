// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "Core/SettingObject.h"
#include "DXCameraShake.generated.h"

/**
 * A Camera Shake object that uses the CameraShakesSetting from DXGameSettings
 */
UCLASS()
class DXGAMESETTINGS_API UDXCameraShake : public UCameraShakeBase
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION()
	void OnSettingChanged(USettingObject* SettingObject);
	
	void StartShake(APlayerCameraManager* Camera, float Scale, ECameraShakePlaySpace InPlaySpace, FRotator UserPlaySpaceRot);

	UPROPERTY()
	int Intensity = 1.f;

	FTimerHandle MyTimerHandle;
	UFUNCTION()
	void StopTimer();
};
