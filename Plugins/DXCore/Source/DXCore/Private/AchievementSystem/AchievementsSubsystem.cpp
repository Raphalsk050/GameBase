// Fill out your copyright notice in the Description page of Project Settings.


#include "AchievementSystem/AchievementsSubsystem.h"

#include "OnlineSubsystem.h"
#include "AchievementSystem/AchievRuleInstance.h"
#include "Core/DXCoreGISubsystem.h"
#include "Core/DXCoreSettings.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"

void UAchievementsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency<UDXCoreGISubsystem>();
	
	UE_LOG(AchievementSystem, Display, TEXT("Achievement Subsystem Initialize."));
	
	UDXCoreGISubsystem* gi = GetGameInstance()->GetSubsystem<UDXCoreGISubsystem>();

	//This should only be called when Game Starts
	/*
	if (gi) 
	{
		gi->OnUserSet.AddDynamic(this, &UAchievementsSubsystem::GetAchievementsAndInitialize);
		gi->OnUserReset.AddDynamic(this, &UAchievementsSubsystem::ReInitialize);
	}

	*/
	//This call is not ideal since there's no user 
	//GetAchievementsAndInitialize();
}

void UAchievementsSubsystem::GetAchievementsAndInitialize()
{
	if (!GetGameInstance())
	{
		UE_LOG(AchievementSystem, Error, TEXT("Game Instance has not been found!")); 
		return;
	}

	UDXCoreGISubsystem* gameInstance = Cast<UDXCoreGISubsystem>(GetGameInstance()->GetSubsystem<UDXCoreGISubsystem>());
	UDXCoreSettings* globalSettings = Cast<UDXCoreSettings>(GEngine->GameSingleton);

	if (!globalSettings->IsValidLowLevel())
		UE_LOG(AchievementSystem, Warning, TEXT("Global Settings has not been found!"));

	if (!gameInstance->IsValidLowLevel())
		UE_LOG(AchievementSystem, Warning, TEXT("Couldn't get DXCoreGISubsystem from GameInstance!"));

	if (gameInstance && globalSettings)
	{
		TMap<FName, FAchievementConfig> achievementsMap;
		globalSettings->GetAchievements(achievementsMap);
		
		for (const auto& achievMapEntry : achievementsMap)
		{
			UE_LOG(AchievementSystem, Display, TEXT("Instantiating achievement %s."), *achievMapEntry.Value.KeyName.ToString());
			
			UAchievementInstance* achievInstance;
			achievInstance = NewObject<UAchievementInstance>(gameInstance);
			achievInstance->Initialize(achievMapEntry.Value, achievMapEntry.Key);
			achievInstance->OnInstanceUpdated.AddDynamic(this, &UAchievementsSubsystem::RefreshAchievementInstance);
			achievInstances.Add(achievInstance);
		}
	}

	QueryAchievements();

	UE_LOG(AchievementSystem, Display, TEXT("Subsystem has been initialized and is running!"));
}

/**
 * @brief Removes all existing Achievement Instances and re-initializes the system.
 */
void UAchievementsSubsystem::ReInitialize()
{
	UE_LOG(AchievementSystem, Display, TEXT("ReInitializing the Subsystem."));
	
	for (int i = achievInstances.Num() - 1; i >= 0; i--) 
	{
		auto& achievInstance = achievInstances[i];
		achievInstance->OnInstanceUpdated.RemoveDynamic(this, &UAchievementsSubsystem::RefreshAchievementInstance);
		achievInstance->ResetAchievement();
		achievInstances.RemoveAt(i);
	}

	achievInstances.Empty(32);	
	GetAchievementsAndInitialize();
}

void UAchievementsSubsystem::RefreshAchievementInstance(FName key, float progress)
{
	UE_LOG(AchievementSystem, Display, TEXT("Refresh was called from an Achievement Instance, sending changes to Online Subsystem."));
	
	UpdateAchievementProgress(key.ToString(), progress);
}

void UAchievementsSubsystem::QueryAchievements()
{
	UDXCoreGISubsystem* gi = Cast<UDXCoreGISubsystem>(GetGameInstance());
	IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get();
	if(gi && onlineSubsystem)
	{
		IOnlineIdentityPtr onlineIdentity = onlineSubsystem->GetIdentityInterface();

		if(onlineIdentity.IsValid())
		{
			TSharedPtr<const FUniqueNetId> userId = gi->GetPlayerUniqueNetId(gi->GetActiveLocalPlayer());
			IOnlineAchievementsPtr onlineAchievements = onlineSubsystem->GetAchievementsInterface();

			if(onlineAchievements.IsValid())
			{
				onlineAchievements->QueryAchievements(*userId.Get(), FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &UAchievementsSubsystem::OnQueryAchievementsComplete));
			}
		}
	}
}

void UAchievementsSubsystem::OnQueryAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		UE_LOG(AchievementSystem, Display, TEXT("Achievements were cached."));
	}
	else
	{
		UE_LOG(AchievementSystem, Warning, TEXT("Failed to cache Achievements."));
	}
}

void UAchievementsSubsystem::UpdateAchievementProgress(const FString& Id, float Percent)
{
	if (Percent < 1.0f)
		return;
		
	//@TODO Integration with Achievement Platform Code	
}