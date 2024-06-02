// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DXCoreSettings.h"

#include "DXCoreFunctionLibrary.h"
#include "AchievementSystem/AchievementSystemTypes.h"

UDXCoreSettings::UDXCoreSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), SaveGameTargetVersion("1.0"), SaveGameVersions({{"1.0",FDXCoreSaveGameVersion()}})
{
	EnablePerformanceTracking = false;
	EnableLocalAnalytics = true;
	LocalAnalyticsSavePath = "Analytics/";
	DestinationAPIURL = "https://luuchowl.com/automation/dx/SendAnalytics.php";
}

void UDXCoreSettings::GetAchievements(TMap<FName, FAchievementConfig>& achievementsMap)
{
	achievementsMap.Reset();

	UDataTable* achievementsTable = AchievementsDataTable.LoadSynchronous();
	
	if (IsValid(achievementsTable))
	{
		achievementsMap = UDXCoreFunctionLibrary::GetStructMap<FAchievementConfig>(achievementsTable);
	}
	else
	{
		UE_LOG(AchievementSystem, Warning, TEXT("Could not load AchievementsDataTable. Please Check DXCore project settings."));
	}
}