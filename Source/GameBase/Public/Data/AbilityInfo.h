// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Core/AbilitySystem/GameplayAbilityBase.h"
#include "AbilityInfo.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class GAMEBASE_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TMap<TSubclassOf<UGameplayAbilityBase>,UInputAction*> Abilities;
};
