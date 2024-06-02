// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "SaveSystem/DXCoreSaveGame.h"
#include "UObject/Object.h"
#include "DXCoreSettings.generated.h"

struct FAchievementConfig;

USTRUCT(BlueprintType)
struct FDXCoreSaveGameVersion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDXCoreMainSaveGame> MainSaveGameClass = UDXCoreMainSaveGame::StaticClass();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDXCoreSlotSaveGame> SlotSaveGameClass = UDXCoreSlotSaveGame::StaticClass();	
};

UCLASS(config = Game, defaultconfig)
class DXCORE_API UDXCoreSettings : public UObject
{
	GENERATED_BODY()
public:
	UDXCoreSettings(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(config, EditAnywhere, Category="Save System", meta=(ConfigRestartRequired=true))
	FString SaveGameTargetVersion;
	
	UPROPERTY(config, EditAnywhere, Category="Save System", meta=(ConfigRestartRequired=true))
	TMap<FString, FDXCoreSaveGameVersion> SaveGameVersions;

	UPROPERTY(config, EditAnywhere, Category = "Achievement System")
	TSoftObjectPtr<UDataTable> AchievementsDataTable;
	
	/**
	 * @brief Returns an achievement TMap of type FAchievementConfig based on the UDataTable provided in the Plugin Settings.
	 * @param Map The map that should be filled with the achievement Map data.
	 */
	void GetAchievements(TMap<FName, FAchievementConfig> &Map);


	UPROPERTY(config, EditAnywhere, Category="Analytics", meta=(ConfigRestartRequired=true))
	bool EnablePerformanceTracking;
	
	UPROPERTY(config, EditAnywhere, Category="Analytics", meta=(ConfigRestartRequired=true))
	FString LocalAnalyticsSavePath;

	UPROPERTY(config, EditAnywhere, Category="Analytics", meta=(ConfigRestartRequired=true))
	bool EnableLocalAnalytics;

	/* the url of the API we want to send the json containing analytics to */
	UPROPERTY(config, EditAnywhere, Category="Analytics", meta=(ConfigRestartRequired=true))
	FString DestinationAPIURL;

	UPROPERTY(config, EditAnywhere, Category="Analytics", meta=(ConfigRestartRequired=false))
	bool ShowLogs;
};