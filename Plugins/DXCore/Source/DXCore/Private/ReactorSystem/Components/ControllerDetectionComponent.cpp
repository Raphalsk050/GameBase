// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorSystem/Components/ControllerDetectionComponent.h"

#include "ReactorSystem/Reactor.h"


// Sets default values for this component's properties
UControllerDetectionComponent::UControllerDetectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UControllerDetectionComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
}

void UControllerDetectionComponent::Detect()
{
	AController* controller = Cast<AController>(GetOwner());
	if (!controller->IsValidLowLevelFast()) return;

	APawn* character = controller->GetPawn();
	if (!character->IsValidLowLevelFast()) return;

	//-- Get All Reactors overlapped by the Pawn.
	TArray<AActor*> overlappingActors;
	character->GetOverlappingActors(overlappingActors, AReactor::StaticClass());
	TArray<AReactor*> ovelappingReactors;
	for (AActor* overlappingActor : overlappingActors)
	{
		AReactor* reactor = Cast<AReactor>(overlappingActor);
		if (reactor->IsValidLowLevelFast()/* && detectionComponent->IsValidLowLevelFast()*/)
		{
			ovelappingReactors.Add(reactor);
		}
	}


	for (AReactor* reactor : ovelappingReactors)
	{
		if (!reactorsBuffer.Contains(reactor))
		{
			//Todo: run start detection
			reactorsBuffer.Add(reactor);
		}

		else
			reactor->Detect(controller);


	}
	for (int i = reactorsBuffer.Num() - 1; i >= 0; i--)
	{
		if (!ovelappingReactors.Contains(reactorsBuffer[i]))
		{
			if(reactorsBuffer[i])
			{
				//Todo: perhaps this is not the best way of finishing detection, but it works...
				reactorsBuffer[i]->Detect(controller);
			}
			reactorsBuffer.RemoveAt(i);
		}
	}
	
	activeKeyLocators.Empty();

	for (AReactor* reactor : reactorsBuffer)
	{
		activeKeyLocators.Append(reactor->GetActiveKeyLocators());
	}
	
}

float UControllerDetectionComponent::GetDistanceToCameraForwardVector(AActor* Actor) const
{
	if (!Actor) return 0.0f;

	APlayerController* playerController = Cast<APlayerController>(GetOwner());
	if (!playerController->IsValidLowLevelFast()) return 0.0f;

	const FVector CameraLocation = playerController->PlayerCameraManager->GetCameraLocation();
	const FVector CameraForwardVector = playerController->PlayerCameraManager->GetCameraRotation().Vector();
	const FVector ActorToCamera = Actor->GetActorLocation() - CameraLocation;

	// Calculate the projection of ActorToCamera onto the CameraForwardVector
	const FVector ProjectedOnForwardVector = FVector::DotProduct(ActorToCamera, CameraForwardVector) * CameraForwardVector;

	// Calculate the vector from the projected point to the actor location
	const FVector DistanceVector = ActorToCamera - ProjectedOnForwardVector;

	// Get the length of the DistanceVector as the distance
	return DistanceVector.Size();
}

float UControllerDetectionComponent::GetDistanceToPawnForwardVector(AActor* Actor) const
{
	if (!Actor) return 0.0f;

	AController* controller = Cast<AController>(GetOwner());
	if (!controller->IsValidLowLevelFast()) return 0.0f;

	APawn* pawn = Cast<APawn>(controller->GetPawn());
	if (!pawn->IsValidLowLevelFast()) return 0.0f;

	const FVector PawnLocation = pawn->GetActorLocation();
	const FVector PawnForwardVector = pawn->GetActorRotation().Vector();
	const FVector ActorToPawn = Actor->GetActorLocation() - PawnLocation;

	// Calculate the projection of ActorToCamera onto the CameraForwardVector
	const FVector ProjectedOnForwardVector = FVector::DotProduct(ActorToPawn, PawnForwardVector) * PawnForwardVector;

	// Calculate the vector from the projected point to the actor location
	const FVector DistanceVector = ActorToPawn - ProjectedOnForwardVector;

	// Get the length of the DistanceVector as the distance
	return DistanceVector.Size();
}

bool UControllerDetectionComponent::HasInteractables()
{
	bool hasInteractables = false;
	for(const auto& reactor : reactorsBuffer)
	{
		if(reactor && reactor->IsValidLowLevel())
		{
			hasInteractables |= reactor->CanInteract();
		}
	}

	return hasInteractables;
}

bool UControllerDetectionComponent::HasInteractablesDetectingTrue(FName InteractTag)
{
	AController* controller = Cast<AController>(GetOwner());
	if (!controller->IsValidLowLevelFast()) return false;
	
	bool hasInteractables = false;
	for(const auto& reactor : reactorsBuffer)
	{
		if(reactor && reactor->IsValidLowLevel())
		{
			hasInteractables |= reactor->CanInteract() && reactor->Detect(controller) && (InteractTag == reactor->InteractTag);
		}
	}

	return hasInteractables;
}

bool UControllerDetectionComponent::Interact(FName InteractTag)
{
	if(IsLockedInteract())
		return false;
	
	AController* controller = Cast<AController>(GetOwner());
	if (!controller->IsValidLowLevelFast()) return false;

	TArray<AReactor*> possibleInteract;
	for(auto& reactor : reactorsBuffer)
	{
		if(reactor && reactor->IsValidLowLevel())
		{
			if(reactor->CanInteract())
			{
				if(InteractTag == reactor->InteractTag)
					possibleInteract.Add(reactor);
			}
		}
	}

	if(bSortInteractByPawnForwardInsteadOfCamera)
	{
		possibleInteract.Sort([this](const AActor& A, const AActor& B)
		{
			return GetDistanceToPawnForwardVector(const_cast<AActor*>(&A)) < GetDistanceToPawnForwardVector(const_cast<AActor*>(&B));
		});
	}
	else
	{
		possibleInteract.Sort([this](const AActor& A, const AActor& B)
		{
			return GetDistanceToCameraForwardVector(const_cast<AActor*>(&A)) < GetDistanceToCameraForwardVector(const_cast<AActor*>(&B));
		});
	}
	

	if(possibleInteract.Num() > 0)
		return possibleInteract[0]->Activate(controller);

	return false;
}

bool UControllerDetectionComponent::IsLockedInteract() const
{
	return bLockedInteract;
}

void UControllerDetectionComponent::LockInteract()
{
	bLockedInteract = true;
}

void UControllerDetectionComponent::UnlockInteract()
{
	bLockedInteract = false;
}

TArray<UKeyLocatorComponent*> UControllerDetectionComponent::GetActiveLocatorComponents()
{
	return activeKeyLocators;
}

TArray<AReactor*> UControllerDetectionComponent::GetDetectingReactors()
{
	return reactorsBuffer;
}


// Called every frame
void UControllerDetectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Detect();
	// ...
}

