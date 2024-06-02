// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementSystem/AchievRuleEvals/AchievRuleEvalAll.h"

float UAchievRuleEvalAll::EvalAchievementRule_Implementation(TArray<bool>& requirements)
{
	float completed = 0.0f;
	for (bool requirement : requirements)
	{
		if (requirement)
			completed++;
	}

	float progress = completed/requirements.Num();
	return FMath::Clamp(progress, 0, 1);
}