// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorSystem/Components/Reactions/RTC_SoundPlayer.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void URTC_SoundPlayer::OnReactorActivating_Implementation(AController* controller)
{
	if (!IsValid(SoundToPlay))
		return;
	
	PlayAudio();
}

void URTC_SoundPlayer::PlayAudio()
{
	if (AudioComponent && AudioComponent->IsValidLowLevel() && !AudioComponent->IsBeingDestroyed() && AudioComponent->GetPlayState() == EAudioComponentPlayState::Playing)
		AudioComponent->Stop();
	
	AudioComponent = UGameplayStatics::SpawnSoundAtLocation(GetOwner()->GetWorld(),
		SoundToPlay,
		IsValid(OverrideActor) ? OverrideActor->GetActorLocation() : GetOwner()->GetActorLocation(),
		IsValid(OverrideActor) ? OverrideActor->GetActorRotation() : GetOwner()->GetActorRotation(),
		VolumeMultiplier,
		PitchMultiplier,
		StartTime,
		AttenuationSettings,
		nullptr,
		true);

	if (AudioComponent)
	{
		AudioComponent->bReverb = AllowReverb ? 1 : 0;
		AudioComponent->OcclusionCheckInterval = AllowOcclusion ? AudioComponent->OcclusionCheckInterval : 0;

		AudioComponent->OnAudioFinished.AddUniqueDynamic(this, &URTC_SoundPlayer::FinishedPlaying);
	}
}

void URTC_SoundPlayer::FinishedPlaying()
{
	TimesPlayed++;
	if (TimesPlayed <= LoopTimes)
		PlayAudio();
	else
		TimesPlayed = 0;
}

void URTC_SoundPlayer::OnReactorDeactivating_Implementation(AController* controller)
{
	Super::OnReactorDeactivating_Implementation(controller);

	if(!bPlayOnDeactivation)
		return;
	
	if (!IsValid(SoundToPlay))
		return;
	
	PlayAudio();
}
