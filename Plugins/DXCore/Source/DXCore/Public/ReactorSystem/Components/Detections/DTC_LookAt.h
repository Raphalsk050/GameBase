// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReactorSystem/Components/DetectionComponent.h"
#include "DTC_LookAt.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Detection), meta = (BlueprintSpawnableComponent))
class DXCORE_API UDTC_LookAt : public UDetectionComponent
{
	GENERATED_BODY()

public:
	UDTC_LookAt();

	UPROPERTY(EditAnywhere, Category = "Config")
	float ScreenPercentTolerance;

	virtual bool DetectInternal_Implementation(AController* controller) override;

protected:

	bool IsLookingAtReactor = false;
	
	UFUNCTION(BlueprintCallable)
	bool IsPositionInsideScreenRadius(FVector Position, float ScreenTolerance);
};
