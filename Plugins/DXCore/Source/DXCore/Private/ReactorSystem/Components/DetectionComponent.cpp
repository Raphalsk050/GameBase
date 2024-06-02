// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorSystem/Components/DetectionComponent.h"


// Sets default values for this component's properties
UDetectionComponent::UDetectionComponent() : _bDetectionEnabled(true)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDetectionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UDetectionComponent::DetectInternal_Implementation(AController* controller)
{
	return false;
}


// Called every frame
void UDetectionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UDetectionComponent::Detect(AController* controller)
{
	if(GetDetectionEnabled())
	{
		return DetectInternal(controller);
	}
	else
	{
		//Return true for any detection on disabled detectors since they should not stop any dependent code
		return true;
	}
}

bool UDetectionComponent::GetDetectionEnabled() const
{
	return _bDetectionEnabled;
}

void UDetectionComponent::SetDetectionEnabled(bool newValue)
{
	if(_bDetectionEnabled != newValue)
	{
		_bDetectionEnabled = newValue;
		OnDetectorEnabledChange.Broadcast(this, newValue);
	}
}

