// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementSystem/AchievRuleEval.h"
#include "AchievementSystem/AchievementSystemTypes.h"

float UAchievRuleEval::EvalAchievementRule_Implementation(TArray<bool>& requirements)
 {
	UE_LOG(AchievementSystem, Warning, TEXT("Base EvalAchievementRule Implementation was called. It will always be 0, make sure to use a derived rule!"));
	
 	return 0.0f;
}
