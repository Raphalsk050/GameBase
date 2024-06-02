// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AchievementSystemTypes.h"
#include "AchievRuleInstance.h"
#include "UObject/NoExportTypes.h"
#include "AchievementInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInstanceUpdated, FName, AchievKey, float, progress);

UCLASS()
class DXCORE_API UAchievementInstance : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY()
	FName achievementKeyName;

	FAchievementConfig config;
	
	TArray<UAchievRuleInstance*> rules;

public:
	void Initialize(FAchievementConfig achievConfig, FName keyName);

	/**
	 * @brief Delegate to use whenever the instance has its progress updated.
	 */
	FInstanceUpdated OnInstanceUpdated;

	/**
	* @brief Returns a 0-1 float based on how many rules have been completed. A value above or equal to 1 is considered completed and unlocks the achievement. 
	* @return 0-1 value indicating the amount of progress made.
	*/
	float GetProgress() const;
	
	FAchievementConfig* GetAchievementConfig();

	/**
	* @brief Called whenever a UAchievRuleInstance has its requirement values updated.
	*/
	UFUNCTION()
	void RefreshInstance();
	
	/**
	 * @brief Called whenever the AchievementSystem gets re-initialized.
	 */
	UFUNCTION()
	void ResetAchievement();
	
};

