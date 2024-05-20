// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase/Public/Core/Controller/PlayerControllerBase.h"

#include "Core/PlayerState/PlayerStateBase.h"

APlayerControllerBase::APlayerControllerBase()
{
}

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
	const AActor* ControlledActor = GetPawn();
	APlayerStateBase* PlayerStateBase = Cast<APlayerStateBase>(GetPawn()->GetPlayerState());
	if (IsValid(ControlledActor) && ControlledActor->InputComponent) {
		PlayerStateBase->GetAbilitySystemComponent()->SetInputComponent(Cast<UEnhancedInputComponent>(ControlledActor->InputComponent));
	}
}
