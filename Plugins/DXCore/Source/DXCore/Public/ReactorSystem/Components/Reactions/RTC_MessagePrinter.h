// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReactorSystem/Components/ReactionComponent.h"
#include "RTC_MessagePrinter.generated.h"

/** 
 * Allows printing to screen a message using optional configuration values.
 * Also allows to print it to the Console Log.
 */
UCLASS(ClassGroup=(Reaction), meta=(BlueprintSpawnableComponent))
class DXCORE_API URTC_MessagePrinter : public UReactionComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category= "Config")
	FString Message;
	
	UPROPERTY(EditAnywhere, Category="Config")
	float TimeToDisplay = 1.0f;

	UPROPERTY(EditAnywhere, Category="Config")
	FColor DisplayColor = FColor::Cyan;

	// Logs to console using UE_LOG under TempLog Category
	UPROPERTY(EditAnywhere, Category="Config")
	bool LogToConsole = false;

	UPROPERTY(EditAnywhere, Category="Config")
	FVector2D TextScale = FVector2D::UnitVector;

	virtual void OnReactorActivating_Implementation(AController* controller) override;
};
