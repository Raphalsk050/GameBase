// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AchievementSystem/AchievRuleEval.h"
#include "AchievRuleEvalHalf.generated.h"

/**
 * 
 */
UCLASS()
class DXCORE_API UAchievRuleEvalHalf : public UAchievRuleEval
{
	GENERATED_BODY()

	virtual float EvalAchievementRule_Implementation(UPARAM(ref)TArray<bool>& requirements) override;
};
