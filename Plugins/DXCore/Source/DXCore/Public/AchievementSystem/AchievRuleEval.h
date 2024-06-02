// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AchievRuleEval.generated.h"

UCLASS(Blueprintable)
/**
 * @brief Base class from which all rule evaluations should be derived.
 * This should return a value between 0-1 according to how many requirements are completed and the specific rule eval function.
 */
class DXCORE_API UAchievRuleEval : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	float EvalAchievementRule(UPARAM(ref)TArray<bool>& requirements);
	virtual float EvalAchievementRule_Implementation(UPARAM(ref)TArray<bool>& requirements);
};
