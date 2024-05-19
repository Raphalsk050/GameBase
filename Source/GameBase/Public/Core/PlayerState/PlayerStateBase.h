// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameBase/Public/Core/Components/AbilitySystemComponentBase.h"
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

private:
	UPROPERTY()
	UAbilitySystemComponentBase* _abilitySystemComponentBase;
	
};
