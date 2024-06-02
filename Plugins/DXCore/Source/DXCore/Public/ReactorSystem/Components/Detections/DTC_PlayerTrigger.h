// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ShapeComponent.h"
#include "ReactorSystem/Components/DetectionComponent.h"
#include "DTC_PlayerTrigger.generated.h"


UCLASS(ClassGroup=(Detection), meta=(BlueprintSpawnableComponent))
class DXCORE_API UDTC_PlayerTrigger : public UDetectionComponent
{
	GENERATED_BODY()

public:

	UDTC_PlayerTrigger();

private:
	//List of colliders being taken into account based on the configuration of the ColliderFilterTags
	UPROPERTY(VisibleAnywhere)
	TArray<UShapeComponent*> FilteredColliders;
	
	FName _lastCollidersFilterTag;
	bool isFilterDirty() const { return CollidersFilterTag != _lastCollidersFilterTag; }
	void UpdateFilteredColliders();
protected:

	virtual bool DetectInternal_Implementation(AController* controller) override;
	virtual void BeginPlay() override;
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowRuntimeChangeFilter;
	
	//If not empty, only colliders that are tagged with these tags will be taken into account.
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName CollidersFilterTag;	
	
	virtual void InitializeComponent() override;
	
	#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif
};


