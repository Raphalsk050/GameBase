// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DXCoreImportantTypes.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "AchievementSystemTypes.generated.h"

class UAchievRuleEval;

DECLARE_LOG_CATEGORY_EXTERN(AchievementSystem, Log, All);

/**
 * @brief Defines the numeric, text and boolean requirements for the Achievement to be completed.
 *
 * These requirements are used by FAchievementRule to generate an UAchievRuleInstance to update and track changes via the Requirement System.
 */
USTRUCT(BlueprintType)
struct DXCORE_API FAchievReqData
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Achievements")
	TArray<FRequirementNumeric> NumericRequirements;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Achievements")
	TArray<FRequirementText> TextRequirements;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Achievements")
	TArray<FRequirementBool> BoolRequirements;

	TArray<FRequirement> GetAllRequirements()
	{
		TArray<FRequirement> requirements;
		requirements.Append(NumericRequirements);
		requirements.Append(TextRequirements);
		requirements.Append(BoolRequirements);
		return requirements;
	}
};

/**
 * @brief Defines the data to generate an UAchievRuleInstance at runtime that will evaluate the RequirementData using the RuleEvaluation
 * to return how much progress has been made on the achievement, and if it has been completed.
 */
USTRUCT(BlueprintType)
struct FAchievementRule
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Achievements")
	FAchievReqData RequirementData;

	/**
	 * @brief How this set of requirements will be evaluated to define the progression amount, and the Achievement completed state.
	 * This should use classes derived from UAchievRuleEval, and not the base UAchievRuleEval class itself.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Achievements")
	TSubclassOf<UAchievRuleEval> RuleEvaluation;	
};

/**
 * @brief MetaData of the Achievement, used to create an Achievement Instance at runtime.
 * This should be defined in a DataTable.
 */
USTRUCT(BlueprintType)
struct FAchievementConfig :  public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	/**
	 * @brief The UniqueIdentifier for this Achievement.
	 * This should match the platform/store settings KeyID for the Achievement to be unlocked.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Achievements")
	FName KeyName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Achievements")
	FText Title;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Achievements")
	FText LockedDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Achievements")
	FText UnlockedDescription;

	//An Achievement can be completed by fulfilling different sets of rules independent of each other.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Achievements")
	TArray<FAchievementRule> Rules;
};

//================================================================================
