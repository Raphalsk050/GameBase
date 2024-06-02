// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementSystem/AchievRuleEvals/AchievRuleEvalAny.h"

float UAchievRuleEvalAny::EvalAchievementRule_Implementation(TArray<bool>& requirements)
 {		
 	return requirements.Contains(true) ? 1.0f: 0.0f;
}