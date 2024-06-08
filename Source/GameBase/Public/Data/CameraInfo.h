// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CameraInfo.generated.h"

/**
 * 
 */
UCLASS()
class GAMEBASE_API UCameraInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Feedback")
	float CameraFieldOfView = 90.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Feedback")
	float TargetArmLength = 300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Feedback")
	float CombatAndMovementTargetArmLength = 600.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Feedback")
	int SpeedPassToChangeTargetArmLenght = 2;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Feedback")
	FVector TargetLocationOffset = FVector(0.0f);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	bool bEnableCameraLag = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(EditCondition = "bEnableCameraLag", EditConditionHides), Category = "Movement|CameraLagProperties")
	bool bDrawDebugLagMarkers = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(EditCondition = "bEnableCameraLag", EditConditionHides), Category = "Movement|CameraLagProperties")
	float CameraLagSpeed = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(EditCondition = "bEnableCameraLag", EditConditionHides), Category = "Movement|CameraLagProperties")
	float CameraLagMaxDistance = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Collision")
	bool bDoCollisionTest = false;
};
