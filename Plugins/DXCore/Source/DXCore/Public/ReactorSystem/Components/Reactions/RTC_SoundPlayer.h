// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReactorSystem/Components/ReactionComponent.h"
#include "Sound/SoundCue.h"
#include "RTC_SoundPlayer.generated.h"

//Plays the target sound not attached at Actor Location or in 2DSpace.
UCLASS(ClassGroup=(Reaction), meta=(BlueprintSpawnableComponent))
class DXCORE_API URTC_SoundPlayer : public UReactionComponent
{
	GENERATED_BODY()

private:	
	UPROPERTY(EditAnywhere, Category="Config")
	USoundCue* SoundToPlay;
	
	UPROPERTY(EditAnywhere, Category="Config")
	float VolumeMultiplier = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Config")
	float PitchMultiplier = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Config")
	float StartTime = 0.f;

	UPROPERTY(EditAnywhere, Category="Config")
	//How many times the sound will be repeated after its original play.
	int LoopTimes = 0;
	
	UPROPERTY(EditAnywhere, Category="Config")
	bool bPlayOnDeactivation = false;

	int TimesPlayed = 0;

	UPROPERTY(EditAnywhere, Category="Config")
	//Allows reverb on the audio component level.
	bool AllowReverb = true;
	
	UPROPERTY(EditAnywhere, Category="Config")
	//Allows occlusion interval on the audio component level.
	bool AllowOcclusion = true;

	UPROPERTY(EditAnywhere, Category="Config")
	//If an Override actor is specified, the Audio will be spawned non attached at the override actor's Location
	AActor* OverrideActor;

	UPROPERTY(EditAnywhere, Category="Config")
	USoundAttenuation* AttenuationSettings;

	UAudioComponent* AudioComponent;

	virtual void OnReactorActivating_Implementation(AController* controller) override;
	
protected:
	UFUNCTION(BlueprintCallable)
	void PlayAudio();

	UFUNCTION()
	void FinishedPlaying();
public:
	virtual void OnReactorDeactivating_Implementation(AController* controller) override;
};