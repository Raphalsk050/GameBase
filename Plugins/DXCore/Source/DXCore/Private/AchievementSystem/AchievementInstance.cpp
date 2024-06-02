// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementSystem/AchievementInstance.h"

#include "DXCoreFunctionLibrary.h"
#include "AchievementSystem/AchievementSystemTypes.h"
#include "AchievementSystem/AchievRuleInstance.h"
#include "Core/DXCoreGISubsystem.h"

void UAchievementInstance::Initialize(FAchievementConfig achievConfig, FName keyName)
{
	achievementKeyName = keyName;

	//Rule Creation
	for(const auto& ruleConfig : achievConfig.Rules)
	{
		UAchievRuleInstance* ruleInstance;
		ruleInstance = NewObject<UAchievRuleInstance>(GetOuter());
		ruleInstance->Initialize(ruleConfig);
		ruleInstance->OnRuleStatusChange.AddDynamic(this, &UAchievementInstance::RefreshInstance);
		rules.Add(ruleInstance);
	}	
}

float UAchievementInstance::GetProgress() const
{
	if(rules.Num() == 0) return 0.0f;

	float progress = 0;
	for (const auto& rule : rules)
		progress += rule->Progress;
	
	return progress/rules.Num();
}

FAchievementConfig* UAchievementInstance::GetAchievementConfig()
{
	return &config;
}

/**
 * @brief Called whenever the rules of the achievement changes (progress is made or reset).
 */
void UAchievementInstance::RefreshInstance()
{
	const float achievementProgress = GetProgress(); //Cache to avoid getting progress multiple times.
	
	UE_LOG(AchievementSystem, Display, TEXT("Achievement REFRESH Progress: %f"), achievementProgress);

	//Update instance before setting global values just in case other global systems require on this one being completed.
	OnInstanceUpdated.Broadcast(achievementKeyName, achievementProgress);
	
	if(achievementProgress >= 1.0f)
	{
		UE_LOG(AchievementSystem, Display, TEXT("Achievement UNLOCKED: %s"), *achievementKeyName.ToString());
		
		UWorld* world = GetWorld();
		UDXCoreGISubsystem* coreGISubsystem = nullptr;
		
		if(world)
		{
			coreGISubsystem = world->GetGameInstance<UDXCoreGISubsystem>();

			if (coreGISubsystem)
				coreGISubsystem->SendRequiredBool(achievementKeyName, true, EGlobalRequiredDataPersistance::GRDP_LevelPersist, false);
		}
	}
}

void UAchievementInstance::ResetAchievement()
{
	UE_LOG(AchievementSystem, Display, TEXT("Achievement RESET: %s"), *achievementKeyName.ToString());

	UWorld* world = GetWorld();
	UDXCoreGISubsystem* coreGISubsystem = nullptr;
		
	if(world)
	{
		coreGISubsystem = world->GetGameInstance<UDXCoreGISubsystem>();

		if (coreGISubsystem)
			coreGISubsystem->SendRequiredBool(achievementKeyName, false, EGlobalRequiredDataPersistance::GRDP_LevelPersist, false);
	}
}