// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorSystem/Components/ReactionComponent.h"

#include "ReactorSystem/Reactor.h"


// Sets default values for this component's properties
UReactionComponent::UReactionComponent() : _bReactionEnabled(true)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

bool UReactionComponent::GetReactionEnabled()
{
	return _bReactionEnabled;
}

void UReactionComponent::SetReactionEnabled(bool newValue)
{
	if (_bReactionEnabled != newValue)
	{
		_bReactionEnabled = newValue;
		if (_bReactionEnabled)
		{
			OnReactionEnabled();
		}
		else
		{
			OnReactionDisabled();
		}
	}
}


// Called when the game starts
void UReactionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UReactionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{	
	GetWorld()->GetTimerManager().ClearTimer(delayTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void UReactionComponent::OnReactionEnabled_Implementation()
{
}


void UReactionComponent::OnReactionDisabled_Implementation()
{
}

void UReactionComponent::StartReacting(AController* controller)
{
	if(ReactionDelay == 0.0f)
	{
		OnReactorActivating(controller);
	}
	else
	{
		FTimerDelegate activationDelegate = FTimerDelegate::CreateUObject(this, &UReactionComponent::OnReactorActivating, controller);
		GetWorld()->GetTimerManager().SetTimer(delayTimerHandle, activationDelegate, ReactionDelay, false, ReactionDelay);
	}
}

// Called every frame
void UReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UReactionComponent::OnReactorBeginPlay_Implementation()
{
}

void UReactionComponent::OnReactorActivating_Implementation(AController* controller)
{
}

void UReactionComponent::OnReactorActivationFailed_Implementation(AController* controller)
{
}

void UReactionComponent::OnReactorDeactivating_Implementation(AController* controller)
{
}

void UReactionComponent::OnReactorDeactivationFailed_Implementation(AController* controller)
{
}

void UReactionComponent::OnReactorSetupByState_Implementation(bool Activated, EReactorStateSource stateSource)
{
}

void UReactionComponent::OnReactorStoreState_Implementation(UParameterKeeper* ParameterKeeper,
	ESaveStateStoreOperation Operation)
{
}

void UReactionComponent::OnReactorSetupByParameters_Implementation(UParameterKeeper* ParameterKeeper,
	ESaveStateSetupOperation Operation)
{
}

void UReactionComponent::OnRegister()
{
	Super::OnRegister();
	reactorOwner = GetOwner() ? CastChecked<AReactor>(GetOwner()) : NULL;
}

void UReactionComponent::InitializeComponent()
{
	Super::InitializeComponent();
	reactorOwner = GetOwner() ? CastChecked<AReactor>(GetOwner()) : NULL;
}

