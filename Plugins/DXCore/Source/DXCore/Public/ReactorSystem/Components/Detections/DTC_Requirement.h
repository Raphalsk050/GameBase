// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DXCoreImportantTypes.h"
#include "ReactorSystem/Components/DetectionComponent.h"
#include "RequirementSystem/Requirable.h"
#include "DTC_Requirement.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Detection), meta = (BlueprintSpawnableComponent), Blueprintable)
class DXCORE_API UDTC_Requirement : public UDetectionComponent, public IRequirable
{
	GENERATED_BODY()
	UDTC_Requirement();
	
protected:
	virtual void BeginPlay() override;

private:
	
	TArray<FRequirementNumeric> NumReqs;
	TArray<FRequirementText> TextReqs;
	TArray<FRequirementBool> BoolReqs;
	TMap<FName, bool> ContinuousReqs;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Requirement Detector", meta = (DisplayAfter="BoolRequirements"))
	TArray<FRequirementNumeric> NumericRequirements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Requirement Detector", meta = (DisplayAfter="NumericRequirements"))
	TArray<FRequirementText> TextRequirements;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Requirement Detector")
	TArray<FRequirementBool> BoolRequirements;

	/**Keep alive when the requirement is met*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Requirement Detector")
	uint8 bUseContinuousRequirements:1;

	/**Reset all requirements when level resets*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Requirement Detector")
	uint8 bResetRequirementsWhenRestart : 1;

	virtual bool DetectInternal_Implementation(AController* controller) override;

	UFUNCTION(BlueprintCallable, Category = "Requirement Detector")
	void ResetRequirements();

	UFUNCTION()
	void ResetContinuousRequirements(TArray<FRequirementNumeric> _numReqs,TArray<FRequirementText> _textReqs,
	TArray<FRequirementBool>_boolReqs);

	UFUNCTION()
	void RegisterAllRequirementsForGlobalRequirements(TArray<FRequirementNumeric> _numericRequirements,
	TArray<FRequirementText> _textRequirements, TArray<FRequirementBool> _boolRequirements);

	virtual void ReceiveGlobalRequirementConcluded_Implementation(FRequirement RequirementValue) override;
	
	virtual void ReceiveGlobalRequirementChanged_Implementation(FRequirement RequirementValue, bool isRequirementMet) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
