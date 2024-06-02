// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/DXCameraShake.h"
#include "Camera/DXCameraShakesSetting.h"
#include "Core/DXGameSettingsSubsystem.h"

void UDXCameraShake::OnSettingChanged(USettingObject* SettingObject)
{
	int value = SettingObject->GetCurrentValueAsInt();
	if (value == 0)
	{
		StopShake(true);
	}
}

void UDXCameraShake::StartShake(APlayerCameraManager* Camera, float Scale, ECameraShakePlaySpace InPlaySpace, FRotator UserPlaySpaceRot)
{
	UDXGameSettingsSubsystem* Subsystem = Camera->GetGameInstance()->GetSubsystem<UDXGameSettingsSubsystem>();
	FString settingsValue = Subsystem->GetSettingCurrentValue(UDXCameraShakesSetting::StaticClass());
	Intensity = FCString::Atoi(ToCStr(settingsValue));;
	Scale *= Intensity;
	GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, this, &UDXCameraShake::StopTimer, GetCameraShakeDuration().Get(), false);
	if (auto delegate = Subsystem->GetSettingDelegate(UDXCameraShakesSetting::StaticClass()))
	{
		delegate->AddDynamic(this, &UDXCameraShake::OnSettingChanged);
	}
	Super::StartShake(Camera, Scale, InPlaySpace, UserPlaySpaceRot);
}

void UDXCameraShake::StopTimer()
{
	UDXGameSettingsSubsystem* Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UDXGameSettingsSubsystem>();
	if (auto delegate = Subsystem->GetSettingDelegate(UDXCameraShakesSetting::StaticClass()))
	{
		delegate->RemoveDynamic(this, &UDXCameraShake::OnSettingChanged);
	}
	if (MyTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(MyTimerHandle);
	}
}