// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DXCoreImportantTypes.h"
#include "UObject/Object.h"
#include "InstantCosmeticEvent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DXCORE_API UInstantCosmeticEvent : public UObject
{
	GENERATED_BODY()

	UInstantCosmeticEvent();

public:
	
	UWorld* WorldContext;
	
	UFUNCTION(BlueprintNativeEvent, Category="Cosmetic Event", meta = (HidePin="WorldContextObject", WorldContext = "WorldContextObject"))
	bool Execute(FCosmeticEventData ParameterKeeperData, const UObject* WorldContextObject);
	virtual bool Execute_Implementation(FCosmeticEventData ParameterKeeperData, const UObject* WorldContextObject);
	
	virtual UWorld* GetWorld() const override;
};
