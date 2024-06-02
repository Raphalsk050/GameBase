// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase/Subsystem/Public/RequiredComponent.h"
#include "GameBase/InteractionSubsystem.h"


// Sets default values for this component's properties
URequiredComponent::URequiredComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URequiredComponent::BeginPlay()
{
	Super::BeginPlay();
    
	if (UInteractionSubsystem* Subsystem = GetWorld()->GetSubsystem<UInteractionSubsystem>())
	{
		if(!InteractionKey.IsEmpty())
		{
			for(auto CurrentKey : InteractionKey)
			{
				Subsystem->RegisterObserver(CurrentKey.KeyName, CurrentKey.bCurrentValue, CurrentKey.DetectionType, this);
			}
		}
	}
	// ...
	
}

void URequiredComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void URequiredComponent::OnNotify(FString Key)
{
	OnNotifyInternal();
	UE_LOG(LogTemp, Warning, TEXT("%s was triggered!"), *Key);
}

void URequiredComponent::OnNotifyInternal_Implementation()
{
	if(NotifyDelegate.IsBound())
	{
		NotifyDelegate.Broadcast();
	}
}

// Called every frame
void URequiredComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

