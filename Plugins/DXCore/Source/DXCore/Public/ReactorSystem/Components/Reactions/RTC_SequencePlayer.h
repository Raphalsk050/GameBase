// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneSequencePlayer.h"
#include "ReactorSystem/Components/ReactionComponent.h"
#include "RTC_SequencePlayer.generated.h"

//Plays a Sequence object
UCLASS(ClassGroup=(Reaction), meta=(BlueprintSpawnableComponent))
class DXCORE_API URTC_SequencePlayer : public UReactionComponent
{
	GENERATED_BODY()
	
	/*private:
	ALevelSequenceActor* PlayerActor;
	
	UPROPERTY(EditAnywhere, Category="Config")
	ULevelSequence* SequenceToPlay;
	
	UPROPERTY(EditAnywhere, Category="Config")
	bool PlayInReverse;

	UPROPERTY(EditAnywhere, Category="Config")
	//Should this Sequence play repeatedly until this ReactionComponent is Deactivated?
	//Ignored if the Loop property in Settings is enabled.
	bool RepeatWhileActive;

	UPROPERTY(EditAnywhere, Category="Config")
	FMovieSceneSequencePlaybackSettings Settings;

	bool IsActive = false;
public:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;;

	UFUNCTION()
	void PlaySequence();
	
	UFUNCTION()
	void SequenceFinished();
	
	virtual void OnReactorActivating_Implementation(AController* controller) override;
	
	virtual void OnReactorDeactivating_Implementation(AController* controller) override;*/
};