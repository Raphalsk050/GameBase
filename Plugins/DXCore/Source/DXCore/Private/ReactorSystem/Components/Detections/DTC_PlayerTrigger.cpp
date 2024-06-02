// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorSystem/Components/Detections/DTC_PlayerTrigger.h"

#include "Components/ShapeComponent.h"
#include "ReactorSystem/Reactor.h"
#include "ReactorSystem/Components/ControllerDetectionComponent.h"


// Sets default values for this component's properties
UDTC_PlayerTrigger::UDTC_PlayerTrigger()
{
	
}

void UDTC_PlayerTrigger::UpdateFilteredColliders()
{
	TArray<USceneComponent*> allChildren;
	GetChildrenComponents(true, allChildren);
	allChildren = allChildren.FilterByPredicate([this](const USceneComponent* item)
	{
		return (CollidersFilterTag.IsNone() ? true : item->ComponentHasTag(CollidersFilterTag))
				&& item->IsA(UShapeComponent::StaticClass());
	});

	TArray<UShapeComponent*> filteredColliders;
	for(const auto& filteredChild : allChildren)
	{
		filteredColliders.Add(Cast<UShapeComponent>(filteredChild));
	}

	FilteredColliders = filteredColliders;
	_lastCollidersFilterTag = CollidersFilterTag;
}

void UDTC_PlayerTrigger::BeginPlay()
{
	Super::BeginPlay();
	UpdateFilteredColliders();
}

bool UDTC_PlayerTrigger::DetectInternal_Implementation(AController* controller)
{
	TArray<AController*> overlappingControllers;

	if(bAllowRuntimeChangeFilter && isFilterDirty())
	{
		UpdateFilteredColliders();
	}
	
	for(auto& shape : FilteredColliders)
	{
		TArray<AActor*> overlappingPawns;
		shape->GetOverlappingActors(overlappingPawns, APawn::StaticClass());
		for(const auto& actorPawn : overlappingPawns)
		{
			const APawn* pawn = Cast<APawn>(actorPawn);
			if(pawn && pawn->GetController() && pawn->GetController()-IsA(APlayerController::StaticClass()))
			{
				overlappingControllers.Add(pawn->GetController());
			}
		}
	}
	
	return overlappingControllers.Num() > 0;
}

#if WITH_EDITOR
void UDTC_PlayerTrigger::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UpdateFilteredColliders();
}
#endif

void UDTC_PlayerTrigger::InitializeComponent()
{
	Super::InitializeComponent();
	UpdateFilteredColliders();
}

