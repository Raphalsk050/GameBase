// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/DXVolumeSetting.h"

#include "Kismet/GameplayStatics.h"

void UDXVolumeSetting::InitSetting_Implementation(UObject* WorldContextObject)
{
	WorldContextObject->GetWorld()->GetTimerManager().SetTimerForNextTick([this, WorldContextObject]()
	{
		if (this && WorldContextObject)
		{
			Super::InitSetting_Implementation(WorldContextObject);
		}
	});
}

void UDXVolumeSetting::OnValueChanged_Implementation(float NormalizedValue)
{
	Super::OnValueChanged_Implementation(NormalizedValue);

	if (!GetWorld())
		return;
	
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SoundClass, NormalizedValue, 1.f, 0.f, true);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), SoundMix);
}