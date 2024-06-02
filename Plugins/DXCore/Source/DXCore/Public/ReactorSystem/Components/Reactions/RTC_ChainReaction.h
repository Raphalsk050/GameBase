// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReactorSystem/Components/ReactionComponent.h"
#include "RTC_ChainReaction.generated.h"

//Calls Activate method on all target Reactors.
UCLASS(ClassGroup=(Reaction), meta=(BlueprintSpawnableComponent))
class DXCORE_API URTC_ChainReaction : public UReactionComponent
{
	GENERATED_BODY()
private:

	//Actors that will receive the Activate method call.
	UPROPERTY(EditAnywhere, Category="Config")
	TArray<AReactor*> Reactors;
	
	virtual void OnReactorActivating_Implementation(AController* controller) override;
};