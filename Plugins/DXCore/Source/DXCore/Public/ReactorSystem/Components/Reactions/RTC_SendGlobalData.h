// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReactorSystem/Components/ReactionComponent.h"
#include "DXCoreImportantTypes.h"
#include "RTC_SendGlobalData.generated.h"

//Sends Global Data to the Requirement System
UCLASS(ClassGroup=(Reaction), meta=(BlueprintSpawnableComponent))
class DXCORE_API URTC_SendGlobalData : public UReactionComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category="Configs")
	TArray<FIntGlobalData> IntRequirementUpdates;

	UPROPERTY(EditAnywhere, Category="Configs")
	TArray<	FFloatGlobalData> FloatRequirementUpdates;
	
	UPROPERTY(EditAnywhere, Category="Configs")
	TArray<FTextGlobalData> TextRequirementUpdates;
	
	UPROPERTY(EditAnywhere, Category="Configs")
	TArray<FBoolGlobalData> BoolRequirementUpdates;
	
public:
	virtual void OnReactorActivating_Implementation(AController* controller) override;
};