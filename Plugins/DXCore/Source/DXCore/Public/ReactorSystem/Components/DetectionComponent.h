// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DetectionComponent.generated.h"

//Forward declaration
class UDetectionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDetectorEnabledChange, UDetectionComponent*, detector, bool, enabled);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class DXCORE_API UDetectionComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDetectionComponent();

	UPROPERTY(BlueprintAssignable)
	FDetectorEnabledChange OnDetectorEnabledChange;

	UFUNCTION(BlueprintPure, Category = "Detection")
	bool GetDetectionEnabled() const;
	
	UFUNCTION(BlueprintCallable, Category = "Detection")
	void SetDetectionEnabled(bool newValue);

private:

	UPROPERTY(EditAnywhere, Category = "Reaction", meta = (DisplayName = "Detection Enabled"))
	uint8 _bDetectionEnabled : 1;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Override this method to define the condition to unlock this detection 
	UFUNCTION(BlueprintNativeEvent, Category = "Detection")
	bool DetectInternal(AController* controller);
	virtual bool DetectInternal_Implementation(AController* controller);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Detection")
	bool Detect(AController* controller);
};
