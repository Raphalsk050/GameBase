/*// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorSystem/Components/Reactions/RTC_SequencePlayer.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"

void URTC_SequencePlayer::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(SequenceToPlay))
		return;
	
	ULevelSequencePlayer::CreateLevelSequencePlayer(GetOwner()->GetWorld(), SequenceToPlay, Settings, PlayerActor);
	PlayerActor->SequencePlayer->OnFinished.AddDynamic(this, &URTC_SequencePlayer::SequenceFinished);
}

void URTC_SequencePlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(PlayerActor))
		PlayerActor->SequencePlayer->OnFinished.RemoveDynamic(this, &URTC_SequencePlayer::SequenceFinished);
}

void URTC_SequencePlayer::OnReactorActivating_Implementation(AController* controller)
{
	if (!IsValid(SequenceToPlay) || !IsValid(PlayerActor))
		return;

	IsActive = true;	
	PlaySequence();
}

void URTC_SequencePlayer::OnReactorDeactivating_Implementation(AController* controller)
{
	IsActive = false;	
}

void URTC_SequencePlayer::PlaySequence()
{
	if (PlayInReverse)
		PlayerActor->SequencePlayer->PlayReverse();
	else
		PlayerActor->SequencePlayer->Play();
}

void URTC_SequencePlayer:: SequenceFinished()
{
	if (!IsValid(SequenceToPlay) || !IsValid(PlayerActor))
		return;

	if (IsActive && RepeatWhileActive)
	{
		if (PlayInReverse)
			PlayerActor->SequencePlayer->PlayReverse();
		else	
			PlayerActor->SequencePlayer->Play();	
	}
}*/