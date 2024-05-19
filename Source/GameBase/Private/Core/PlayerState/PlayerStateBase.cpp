// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase/Public/Core/PlayerState/PlayerStateBase.h"

APlayerStateBase::APlayerStateBase()
{
	// Create ability system component, and set it to be explicitly replicated
	_abilitySystemComponentBase = CreateDefaultSubobject<UAbilitySystemComponentBase>(TEXT("AbilitySystemComponent"));
	_abilitySystemComponentBase->SetIsReplicated(true);
	//...

	
}

UAbilitySystemComponentBase* APlayerStateBase::GetAbilitySystemComponent()
{
	return _abilitySystemComponentBase;
}
