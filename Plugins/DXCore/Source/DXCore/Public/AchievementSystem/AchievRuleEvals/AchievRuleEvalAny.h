// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AchievementSystem/AchievRuleEval.h"
#include "AchievRuleEvalAny.generated.h"

/**
 * 
 */
UCLASS()
class DXCORE_API UAchievRuleEvalAny : public UAchievRuleEval
{
	GENERATED_BODY()

	/**
	 * @brief Evaluates if the requirements have been completed or not.
	 * @param requirements The array containing the state of all requirements defined, being either true or false for completed. 
	 * @return A value of 0-1 depending on the custom implementation of a rule evaluation. 1 or above means completed, but this value
	 * should always be returned clamped.
	 */
	virtual float EvalAchievementRule_Implementation(UPARAM(ref)TArray<bool>& requirements) override;
};
