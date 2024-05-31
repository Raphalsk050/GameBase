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
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CameraFieldOfView = 90.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetArmLength = 600.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector TargetLocationOffset = FVector(0.0f);
};
