// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineStats.h"
#include "AchievementSystem/AchievementInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AchievementsSubsystem.generated.h"

UCLASS()
class DXCORE_API UAchievementsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<UAchievementInstance*> achievInstances; 
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * @brief Gets achievements from GlobalSettings and generate UAchievementInstances to keep track and update their state.
	 */
	UFUNCTION()
	void GetAchievementsAndInitialize();
	
	UFUNCTION()
	void ReInitialize();

	UFUNCTION()
	void RefreshAchievementInstance(FName key, float progress);

	/* Get all the available achievements and cache them */
	void QueryAchievements();

	/* Called when the cache finishes */
	void OnQueryAchievementsComplete(const FUniqueNetId& PlayerId, const bool bWasSuccessful);

	/* Updates the achievement progress */
	UFUNCTION()
	void UpdateAchievementProgress(const FString& Id, float Percent);

	FOnlineAchievementsWritePtr AchievementsWriteObjectPtr;
};

