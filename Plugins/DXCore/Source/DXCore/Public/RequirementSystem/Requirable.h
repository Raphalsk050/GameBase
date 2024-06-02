// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DXCoreImportantTypes.h"
#include "Requirable.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class URequirable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DXCORE_API IRequirable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Requirement System")
	bool IsRequirementMet(FRequirement requirement) const;
	virtual bool IsRequirementMet_Implementation(FRequirement requirement) const;

	//Dispatches the value for a certain requirement to the GlobalSettings, so , can only be done on Server
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Requirement System", meta = (DisplayName = "OnRequirementConcluded"))
	void ReceiveGlobalRequirementConcluded(FRequirement requirementValue);
	virtual void ReceiveGlobalRequirementConcluded_Implementation(FRequirement requirementValue);

	//Dispatches the continuous value of a requirement
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Requirement System", meta = (DisplayName = "OnRequirementChanged"))
	void ReceiveGlobalRequirementChanged(FRequirement requirementValue, bool isRequirementMet);
	virtual void ReceiveGlobalRequirementChanged_Implementation(FRequirement requirementValue, bool isRequirementMet);
};
