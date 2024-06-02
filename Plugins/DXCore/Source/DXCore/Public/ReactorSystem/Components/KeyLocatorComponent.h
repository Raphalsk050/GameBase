// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReactorSystem/Reactor.h"

#include "KeyLocatorComponent.generated.h"

UENUM(BlueprintType)
enum class ELocatorReactorRequirement : uint8
{
	LRR_Deactivated	UMETA(DisplayName = "Deactivated"),
	LRR_Activated	UMETA(DisplayName = "Activated"),
	LRR_Any	UMETA(DisplayName = "Any")
};
/**
 * This component was made as an agnostic abstraction of representing an input hint on a Reactor, where you could
 * possibly want it to show on just some of the Detectors from the Reactor are met, like a bigger area before you can
 * really interact with the Reactor. You can access which are the active locators by the ControllerDetectionComponent
 * and identify them by their Identifier and them act on that information. A simple default Show/Hide was also implemented
 * directly on it, but for any custom implementation of what it should do when active should be done by getting them from
 * the ControllerDetectionComponent.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class DXCORE_API UKeyLocatorComponent : public USceneComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TMap<UDetectionComponent*, EDetectionConfig> detectionConfigs;

	UPROPERTY()
	TMap<UPrimitiveComponent*, TEnumAsByte<ECollisionEnabled::Type>> initialCollisionConfig;
	
	UPROPERTY()
	TMap<USceneComponent*, bool> initialVisibilityConfig;

	bool bReadyToDetect;

	bool bActive;
	
	void SetupByActive();
public:
	//This will help you identify the type of locator for a specific purpose
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Identifier;

	//The state in which the Reactor must be for it to be active
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELocatorReactorRequirement ReactorMustBe;

	//Todo: implement a minimal activation (OR Condition)
	
	//The detectors that must detect as true for this locator to be active (AND condition only for now)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, EDetectionConfig> RequireDetectors;

	//When true, it will try to hide and turn collisions off for self and all children
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseActiveToShowAndHideSelf;

	//This will start the locator as active until it is detected again/first time.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bStartActive;

	void OnReactorBeginPlay();

	bool IsLocatorActive() const { return bActive; }

	bool DetectUpdateActiveState(AController* controller, bool ReactorActivated, bool ReactorChangingState);
};
