// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FollowCamera.h"
#include "Core/AbilitySystem/GameplayAbilityBase.h"
#include "Data/AbilityInfo.h"
#include "Data/CameraInfo.h"
#include "GameBase/Public/Core/Character/CharacterBase.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMEBASE_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerControllerBase();

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAbilityInfo* CharacterDefaultAbilities;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCameraInfo* CameraInfo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AFollowCamera* FollowCamera;
};
