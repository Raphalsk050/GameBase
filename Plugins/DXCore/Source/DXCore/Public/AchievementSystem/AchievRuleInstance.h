// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AchievementSystemTypes.h"
#include "RequirementSystem/Requirable.h"
#include "UObject/NoExportTypes.h"
#include "AchievRuleInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRuleDelegate);

UCLASS()
/**
 * @brief Runtime rule instance responsible for receiving updates to the required values and updating the progress of the achievement.
 **/
class DXCORE_API UAchievRuleInstance : public UObject, public IRequirable
{
	GENERATED_BODY()

private:
	FAchievementRule* rule;

	TSubclassOf<UAchievRuleEval> evalClass;

public:
	float Progress;

	TMap<FName, bool> requirementsMap;

	FRuleDelegate OnRuleStatusChange;

	void Initialize(FAchievementRule ruleConfig);

	/**
	 * @brief Implementation of Requirement system's requirement update.
	 * 
	 * This triggers an update and re-evaluation of progress of the achievement based on its requirements, which in turn broadcasts
	 * its updated value to the UAchievementInstance that will mark the Achievement as Completed.
	 * @param requirementValue The requirement object that is being tracked.
	 * @param isRequirementMet The new value received from the requirement system.
	 */
	virtual void ReceiveGlobalRequirementChanged_Implementation(FRequirement requirementValue, bool isRequirementMet) override;
};

