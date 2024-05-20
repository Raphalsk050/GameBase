// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameBase/Public/Core/Components/AbilitySystemComponentBase.h"
#include "Core/AbilitySystem/CharacterAttributeSetBase.h"
#include "Core/AbilitySystem/GameplayAbilityBase.h"
#include "Abilities/GameplayAbility.h"
#include "PlayerStateBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMEBASE_API APlayerStateBase : public APlayerState
{
	GENERATED_BODY()
public:
	APlayerStateBase();

	UFUNCTION(BlueprintCallable)
	UAbilitySystemComponentBase* GetAbilitySystemComponent();

	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();

	//AttributeSet Getters
	UFUNCTION(BlueprintCallable, Category = "GameBase|Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "GameBase|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GameBase|Attributes")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "GameBase|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GameBase|Attributes")
	float GetMaxStamina() const;
	
	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "GameBase|Attributes")
	float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "GameBase|Attributes")
	float GetMoveSpeedBaseValue() const;

	// Switch on AbilityID to return individual ability levels. Hardcoded to 1 for every ability in this project.
	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDCharacter")
	virtual int32 GetAbilityLevel(EGameBaseAbilityInputID AbilityID) const;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (HideInDetailPanel))
	UAbilitySystemComponentBase* AbilitySystemComponentBase;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TWeakObjectPtr<class UCharacterAttributeSetBase> AttributeSetBase;

	//TODO: Add the character bank data to get current character information like name, relationships...
	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GameBase|Abilities")
	TArray<TSubclassOf<class UGameplayAbilityBase>> CharacterAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GameBase|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GameBase|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	virtual void AddStartupEffects();
};
