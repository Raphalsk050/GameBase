// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementSystem/AchievRuleEvals/AchievRuleEvalQuarter.h"

float UAchievRuleEvalQuarter::EvalAchievementRule_Implementation(TArray<bool>& requirements)
{
	float completed = 0.0f;
	for (bool requirement : requirements)
	{
		if (requirement)
			completed++;
	}

	float progress = completed/requirements.Num();
	progress /= .25f;
	
	return FMath::Clamp(progress, 0, 1);
}