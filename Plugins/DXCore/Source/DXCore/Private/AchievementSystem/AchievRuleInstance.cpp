// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementSystem/AchievRuleInstance.h"

#include "AchievementSystem/AchievRuleEval.h"
#include "Core/DXCoreGISubsystem.h"

void UAchievRuleInstance::Initialize(FAchievementRule ruleConfig)
{
	rule = &ruleConfig;
	evalClass = ruleConfig.RuleEvaluation;

	UDXCoreGISubsystem* gi = Cast<UDXCoreGISubsystem>(GetOuter());
	if (gi)
	{
		for (FRequirementNumeric req : ruleConfig.RequirementData.NumericRequirements)
		{
			requirementsMap.Add(req.RequiredDataName, false);
		}
		for (FRequirementText req : ruleConfig.RequirementData.TextRequirements)
		{
			requirementsMap.Add(req.RequiredDataName, false);
		}
		for (FRequirementBool req : ruleConfig.RequirementData.BoolRequirements)
		{
			requirementsMap.Add(req.RequiredDataName, false);
		}

		for (FRequirementNumeric req : ruleConfig.RequirementData.NumericRequirements)
		{
			gi->RegisterForGlobalRequirement(req.ToRequirement(), this, true);
			UE_LOG(AchievementSystem, Display, TEXT("Rule Initialized: %s"), *req.RequiredDataName.ToString());
		}
		for (FRequirementText req : ruleConfig.RequirementData.TextRequirements)
		{
			gi->RegisterForGlobalRequirement(req.ToRequirement(), this, true);
			UE_LOG(AchievementSystem, Display, TEXT("Rule Initialized: %s"), *req.RequiredDataName.ToString());
		}
		for (FRequirementBool req : ruleConfig.RequirementData.BoolRequirements)
		{
			gi->RegisterForGlobalRequirement(req.ToRequirement(), this, true);
			UE_LOG(AchievementSystem, Display, TEXT("Rule Initialized: %s"), *req.RequiredDataName.ToString());
		}
	}
}

void UAchievRuleInstance::ReceiveGlobalRequirementChanged_Implementation(FRequirement requirementValue, bool isRequirementMet)
{
	//Update the specific requirement in the requirements map.
	requirementsMap.Add(requirementValue.RequiredDataName, isRequirementMet);

	//We only want to check the value of the requirements, so we create an array containing just that.
	TArray<bool> completedRequirements;
	requirementsMap.GenerateValueArray(completedRequirements);

	//Check is valid to prevent a crash.
	if (IsValid(evalClass))
	{
		//Evaluates progress based on the EvalAchievRule that was setup in the FAchievementConfig
		Progress = evalClass.GetDefaultObject()->EvalAchievementRule(completedRequirements);

		UE_LOG(AchievementSystem, Display, TEXT("Achievement Requirement was changed to %s; progress: %f"), isRequirementMet? TEXT("true") : TEXT("false"), Progress);
	
		OnRuleStatusChange.Broadcast();
	}
	else
	{
		UE_LOG(AchievementSystem, Warning, TEXT("An Achiev Rule Config has no AchievRuleEval Defined! It will not be executed!"));
	}
}