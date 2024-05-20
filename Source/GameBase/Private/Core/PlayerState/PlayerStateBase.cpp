// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase/Public/Core/PlayerState/PlayerStateBase.h"


APlayerStateBase::APlayerStateBase()
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponentBase = CreateDefaultSubobject<UAbilitySystemComponentBase>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponentBase->SetIsReplicated(true);
	//...
}

UAbilitySystemComponentBase* APlayerStateBase::GetAbilitySystemComponent()
{
	return AbilitySystemComponentBase;
}

void APlayerStateBase::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponentBase->IsValidLowLevelFast())
	{
		return;
	}

	// Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponentBase->GetActivatableAbilities())
	{
		if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	// Do in two passes so the removal happens after we have the full list
	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponentBase->ClearAbility(AbilitiesToRemove[i]);
	}
}

int32 APlayerStateBase::GetCharacterLevel() const
{
	if (AttributeSetBase.IsValid())
	{
		return static_cast<int32>(AttributeSetBase->GetCharacterLevel());
	}

	return 0;
}

float APlayerStateBase::GetHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetHealth();
	}

	return 0.0f;
}

float APlayerStateBase::GetMaxHealth() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxHealth();
	}

	return 0.0f;
}

float APlayerStateBase::GetStamina() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetStamina();
	}

	return 0.0f;
}

float APlayerStateBase::GetMaxStamina() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxStamina();
	}

	return 0.0f;
}

float APlayerStateBase::GetMoveSpeed() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxWalkSpeed();
	}

	return 0.0f;
}

float APlayerStateBase::GetMoveSpeedBaseValue() const
{
	if (AttributeSetBase.IsValid())
	{
		return AttributeSetBase->GetMaxWalkSpeed();
	}

	return 0.0f;
}

int32 APlayerStateBase::GetAbilityLevel(EGameBaseAbilityInputID AbilityID) const
{
	return 1;
}

void APlayerStateBase::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponentBase->IsValidLowLevelFast())
	{
		return;
	}

	for (TSubclassOf<UGameplayAbilityBase>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponentBase->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID), static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}
}

void APlayerStateBase::InitializeAttributes()
{
}

void APlayerStateBase::AddStartupEffects()
{
}
