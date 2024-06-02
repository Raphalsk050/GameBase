// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorSystem/Components/KeyLocatorComponent.h"

void UKeyLocatorComponent::OnReactorBeginPlay()
{
	TArray<USceneComponent*> allChildren;
	GetChildrenComponents(true, allChildren);

	for(const auto& child : allChildren)
	{
		initialVisibilityConfig.Add(child, child->GetVisibleFlag());
	}
	
	allChildren = allChildren.FilterByPredicate([this](const USceneComponent* item)
	{
		return item->IsA(UPrimitiveComponent::StaticClass());
	});
			
	
	for(const auto& filteredChild : allChildren)
	{
		if(UPrimitiveComponent* child = Cast<UPrimitiveComponent>(filteredChild))
		{
			initialCollisionConfig.Add(child, child->GetCollisionEnabled());
		}
	}
	
	if(RequireDetectors.Num() > 0)
	{
		TArray<UActorComponent*> detectionComponents; 
		GetOwner()->GetComponents(UDetectionComponent::StaticClass(), detectionComponents, false);
		for(auto& detectionComp : detectionComponents)
		{
			if(RequireDetectors.Contains(detectionComp->GetFName()))
			{
				UDetectionComponent* detection = Cast<UDetectionComponent>(detectionComp);
				if(detection)
				{
					detectionConfigs.Add(detection, RequireDetectors[detectionComp->GetFName()]);
				}
			}
		}
	}
	else
	{
		TArray<UActorComponent*> detectionComponents; 
		GetOwner()->GetComponents(UDetectionComponent::StaticClass(), detectionComponents, false);
		for(auto& detectionComponent : detectionComponents)
		{
			if(UDetectionComponent* detector = Cast<UDetectionComponent>(detectionComponent))
			{
				detectionConfigs.Add(detector, EDetectionConfig::DC_MustDetectTrue);
			}
		}
	}

	if(bStartActive)
		bActive = true;

	SetupByActive();
	bReadyToDetect = true;
}

void UKeyLocatorComponent::SetupByActive()
{
	if(bUseActiveToShowAndHideSelf)
	{
		this->SetVisibility(bActive);
		
		for(auto& child : initialVisibilityConfig)
		{
			child.Key->SetVisibility(bActive ? child.Value : false);
		}

		
		for(TTuple<UPrimitiveComponent*, TEnumAsByte<ECollisionEnabled::Type>>& child : initialCollisionConfig)
		{
			ECollisionEnabled::Type collision = bActive ? static_cast<ECollisionEnabled::Type>(child.Value) : ECollisionEnabled::NoCollision; 
			child.Key->SetCollisionEnabled(collision);
		}
	}
}

bool UKeyLocatorComponent::DetectUpdateActiveState(AController* controller, bool ReactorActivated,
	bool ReactorChangingState)
{
	if(!bReadyToDetect)
		return false;
	
	if(ReactorChangingState)
	{
		bActive = false;
	}
	else
	{
		bool detectionSuccess = true;
		for(auto& activationDetection: detectionConfigs)
		{
			bool detection = activationDetection.Value == EDetectionConfig::DC_MustDetectTrue ?
																	activationDetection.Key->Detect(controller) :
																	!activationDetection.Key->Detect(controller); 

			detectionSuccess &= detection;
		}

		auto Reactor = Cast<AReactor>(GetOwner());
		bool ReactorTest = false;
		if(ReactorMustBe == ELocatorReactorRequirement::LRR_Activated)
			ReactorTest = Reactor->IsActivated();
		else if(ReactorMustBe == ELocatorReactorRequirement::LRR_Deactivated)
			ReactorTest = !Reactor->IsActivated();
		else
			ReactorTest = true;

		bActive = detectionSuccess && ReactorTest;
	}

	SetupByActive();
	return bActive;
}
