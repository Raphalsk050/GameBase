// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorSystem/Reactor.h"

#include "ReactorSystem/Components/DetectionComponent.h"
#include "ReactorSystem/Components/KeyLocatorComponent.h"
#include "ReactorSystem/Components/ReactionComponent.h"

// Sets default values
AReactor::AReactor() : bIgnoreDetectorsForDeactivation(true), DeactivationConfigRelation(EDetectionConfigRelation::DCR_Or),_bActivated(false), _bEverActivated(false), _bActivationDetectionInfiniteLoopFlag(false), _bDetectingActivationInfiniteLoopFlag(false), _bActivating(0), _bDeactivating(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SAVESTATE_INIT()
	
	if(InitialState && InitialState->IsValidLowLevel())
	{
		//Configure any initial states!
		InitialState->SetBool("Activated", false);
		InitialState->SetBool("EverActivated", false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR! InitialState should be initialized by now"));
	}
	
	if(SavedState && SavedState->IsValidLowLevel())
	{
		//Configure any initial states!
		SavedState->SetBool("Activated", false);
		SavedState->SetBool("EverActivated", false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR! SavedState should be initialized by now"));
	}
}

// Called when the game starts or when spawned
void AReactor::BeginPlay()
{
	Super::BeginPlay();
	if(bAutoDeactivate)
	{
		bIgnoreDetectorsForDeactivation = false;
	}
	
	if(ActivationDetectionOverride.Num() > 0)
	{
		TArray<UActorComponent*> detectionComponents; 
		GetOwner()->GetComponents(UDetectionComponent::StaticClass(), detectionComponents, false);
		for(auto& detectionComp : detectionComponents)
		{
			if(ActivationDetectionOverride.Contains(detectionComp->GetFName()))
			{
				UDetectionComponent* detection = Cast<UDetectionComponent>(detectionComp);
				if(detection)
				{
					activationConfigs.Add(detection, ActivationDetectionOverride[detectionComp->GetFName()]);
				}
			}
		}
	}
	else
	{
		TArray<UActorComponent*> detectionComponents; 
		GetComponents(UDetectionComponent::StaticClass(), detectionComponents, false);
		for(auto& detectionComponent : detectionComponents)
		{
			if(UDetectionComponent* detector = Cast<UDetectionComponent>(detectionComponent))
			{
				activationConfigs.Add(detector, EDetectionConfig::DC_MustDetectTrue);
			}
		}
	}
	
	if(DeactivationDetectionOverride.Num() > 0)
	{
		TArray<UActorComponent*> detectionComponents; 
		GetOwner()->GetComponents(UDetectionComponent::StaticClass(), detectionComponents, false);
		for(auto& detectionComp : detectionComponents)
		{
			if(DeactivationDetectionOverride.Contains(detectionComp->GetFName()))
			{
				UDetectionComponent* detection = Cast<UDetectionComponent>(detectionComp);
				if(detection)
				{
					deactivationConfigs.Add(detection, DeactivationDetectionOverride[detectionComp->GetFName()]);
				}
			}
		}
	}
	else
	{
		TArray<UActorComponent*> detectionComponents; 
		GetComponents(UDetectionComponent::StaticClass(), detectionComponents, false);
		for(auto& detectionComponent : detectionComponents)
		{
			if(UDetectionComponent* detector = Cast<UDetectionComponent>(detectionComponent))
			{
				deactivationConfigs.Add(detector, EDetectionConfig::DC_MustDetectFalse);
			}
		}
	}

	//Locators Setup
	TArray<UActorComponent*> keyLocatorActorComponents; 
	GetComponents(UKeyLocatorComponent::StaticClass(), keyLocatorActorComponents, false);
	for(auto& keyLocator : keyLocatorActorComponents)
	{
		if(UKeyLocatorComponent* keyLocatorComponent = Cast<UKeyLocatorComponent>(keyLocator))
		{
			keyLocatorComponents.Add(keyLocatorComponent);
			keyLocatorComponent->OnReactorBeginPlay();
		}
	}
	

	TArray<UReactionComponent*> reactions;
	GetComponents(reactions, false);
	for(auto& reaction : reactions)
	{
		if(reaction->GetReactionEnabled())
		{
			reaction->OnReactorBeginPlay();
		}
	}
	
	Execute_SaveInitialState(this);
}

// Called every frame
void AReactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AReactor::Detect(AController* controller)
{
	if(_bActivating > 0 || _bDeactivating > 0)
	{
		for(auto& keyLocator : keyLocatorComponents)
		{
			keyLocator->DetectUpdateActiveState(controller, IsActivated(), true);
		}
		return false;
	}

	const bool _bMinimalActivation = ActivationConfigRelation == EDetectionConfigRelation::DCR_Or;
	const bool _bMinimalDeactivation = DeactivationConfigRelation == EDetectionConfigRelation::DCR_Or;
	
	_bDetectingActivationInfiniteLoopFlag = true;

	for(auto& keyLocator : keyLocatorComponents)
	{
		keyLocator->DetectUpdateActiveState(controller, IsActivated(), false);
	}
	
	if(!IsActivated())
	{
		//MinimalActivation inverts the relations of each DetectionConfig from AND to OR.  
		bool success = !_bMinimalActivation;
		for(auto& activationDetection: activationConfigs)
		{
			bool detection = activationDetection.Value == EDetectionConfig::DC_MustDetectTrue ?
																	activationDetection.Key->Detect(controller) :
																	!activationDetection.Key->Detect(controller); 
			if(_bMinimalActivation)
				success |= detection;
			else
				success &= detection;
		}
		if(success)
		{
			if(bAutoEnableInput)
			{
				APlayerController* playerController = Cast<APlayerController>(controller);
				if(playerController)
				{
					EnableInput(playerController);
				}
			}
			//Prevent AutoActivate if Detect is coming from Activate call stack
			if(bAutoActivate && !_bActivationDetectionInfiniteLoopFlag)
			{
				Activate(controller);
			}
		}
		_bDetectingActivationInfiniteLoopFlag = false;
		return success;
	}

	//Deactivating
	//MinimalDeactivation inverts the relations of each DetectionConfig from AND to OR.  
	bool success = !_bMinimalDeactivation;
	if(!bIgnoreDetectorsForDeactivation)
	{
		for(auto& deactivationDetection: deactivationConfigs)
		{
			bool detection = deactivationDetection.Value == EDetectionConfig::DC_MustDetectTrue ?
																deactivationDetection.Key->Detect(controller) : 
																!deactivationDetection.Key->Detect(controller);
			if(_bMinimalDeactivation)
				success |= detection;
			else
				success &= detection;
		}
	}
	else
	{
		success = true;
	}
	
	if(success)
	{
		if(bAutoDisableInput)
		{
			APlayerController* playerController = Cast<APlayerController>(controller);
			if(playerController)
			{
				DisableInput(playerController);
			}
		}
		//Prevent AutoDeactivate if Detect is coming from Deactivate call stack
		if(bAutoDeactivate && !_bActivationDetectionInfiniteLoopFlag)
		{
			Deactivate(controller);
		}
	}
	_bDetectingActivationInfiniteLoopFlag = false;
	return success;
}

bool AReactor::Activate(AController* controller)
{
	//When running Activate make sure Detect knows not to auto-run Activate again
	_bActivationDetectionInfiniteLoopFlag = true;
	
	bool bPreCheck = true;
	//Make sure it's not in the middle of another activation
	if(_bActivating > 0 || _bDeactivating > 0) { bPreCheck = false; }
	//If Activate is coming from a Detect call stack, don't need to run Detect again
	if(!_bDetectingActivationInfiniteLoopFlag)
	{
		//Make sure all requirements/detectors are met
		if(bPreCheck && Detect(controller) == false) { bPreCheck = false; }
	}
	//Make sure it cannot be reactivated if repeat is false
	if(bPreCheck && (_bEverActivated && bCanRepeat == false)) { bPreCheck = false; }
	//Make sure it cannot be reactivated while active, if deactivation is possible it should be deactivated first.
	if(bPreCheck && (bCanDeactivate && _bActivated)) { bPreCheck = false; }
	
	_bActivationDetectionInfiniteLoopFlag = false;

	TArray<UReactionComponent*> reactions;
	GetComponents(reactions, false);
	
	if(bPreCheck == false)
	{
		ActivationFailed(controller);
		for(auto& reaction : reactions)
		{
			if(reaction->GetReactionEnabled())
			{
				reaction->OnReactorActivationFailed(controller);
			}
		}
		
		return false;
	}

	_bActivated = true;
	_bEverActivated = true;
	
	_bActivating = 1;
	Activating(controller);
	for(auto& reaction:reactions)
	{
		if(reaction->GetReactionEnabled())
		{
			reaction->StartReacting(controller);
		}
	}

	_controllerThatActivatedLast = controller;
	
	if(_bActivating == 2)
	{
		SetupByState(true, EReactorStateSource::RSS_Activating);
		for(auto& reaction:reactions)
		{
			if(reaction->GetReactionEnabled())
			{
				reaction->OnReactorSetupByState(true, EReactorStateSource::RSS_Activating);
			}
		}
		_bActivating = 0;
	}
	else if(_bActivating == 1)
	{
		_bActivating = 2;
	}
	return true;
}

bool AReactor::Deactivate(AController* controller)
{
	//When running Deactivate make sure Detect knows not to auto-run Deactivate again
	_bActivationDetectionInfiniteLoopFlag = true;
	
	bool bPreCheck = true;
	//Make sure it's not in the middle of another activation
	if(_bActivating > 0 || _bDeactivating > 0) { bPreCheck = false; }
	//If Deactivate is coming from a Detect call stack, don't need to run Detect again
	if(!bIgnoreDetectorsForDeactivation && !_bDetectingActivationInfiniteLoopFlag)
	{
		//Make sure all requirements/detectors are met
		if(bPreCheck && Detect(controller) == false) { bPreCheck = false; }
	}
	//Make sure it cannot be deactivated if it's already deactivated
	if(bPreCheck && _bActivated == false) { bPreCheck = false; }
	//Make sure it can be deactivated
	if(bPreCheck && bCanDeactivate == false) { bPreCheck = false; }
	
	_bActivationDetectionInfiniteLoopFlag = false;
	

	TArray<UReactionComponent*> reactions;
	GetComponents(reactions, false);
	
	if(bPreCheck == false)
	{
		DeactivationFailed(controller);
		for(auto& reaction : reactions)
		{
			if(reaction->GetReactionEnabled())
			{
				reaction->OnReactorDeactivationFailed(controller);
			}
		}
		return false;
	}

	_bActivated = false;
	
	_bDeactivating = 1;
	Deactivating(controller);
	for(auto& reaction:reactions)
	{
		if(reaction->GetReactionEnabled())
		{
			reaction->OnReactorDeactivating(controller);
		}
	}

	_controllerThatDeactivatedLast = controller;
	
	if(_bDeactivating == 2)
	{
		SetupByState(false, EReactorStateSource::RSS_Deactivating);
		for(auto& reaction:reactions)
		{
			if(reaction->GetReactionEnabled())
			{
				reaction->OnReactorSetupByState(false, EReactorStateSource::RSS_Deactivating);
			}
		}
		_bDeactivating = 0;
	}
	else if(_bDeactivating == 1)
	{
		_bDeactivating = 2;
	}

	return true;
}

TArray<UKeyLocatorComponent*> AReactor::GetActiveKeyLocators()
{
	TArray<UKeyLocatorComponent*> activeLocators;
	for(auto& locator: keyLocatorComponents)
	{
		if(locator->IsLocatorActive())
		{
			activeLocators.Add(locator);
		}
	}
	return  activeLocators;
}

bool AReactor::CanInteract_Implementation()
{
	if(bPreventInteractionDeactivation && IsActivated())
		return false;

	if(bPreventInteractionActivation && !IsActivated())
		return false;
	
	return bInteractable;
}

bool AReactor::IsActivated() const
{
	return _bActivated;
}

bool AReactor::WasEverActivated() const
{
	return _bEverActivated;
}

void AReactor::Activating_Implementation(AController* controller)
{
	FinishActivating();
}

void AReactor::FinishActivating()
{
	if(_bActivating == 1)
	{
		_bActivating = 2;
	}
	else if(_bActivating == 2)
	{
		SetupByState(true, EReactorStateSource::RSS_Activating);
		TArray<UReactionComponent*> reactions;
		GetComponents(reactions, false);
		for(auto& reaction:reactions)
		{
			if(reaction->GetReactionEnabled())
			{
				reaction->OnReactorSetupByState(true, EReactorStateSource::RSS_Activating);
			}
		}
		_bActivating = 0;
	}
}

void AReactor::ActivationFailed_Implementation(AController* controller)
{
	
}

void AReactor::Deactivating_Implementation(AController* controller)
{
	FinishDeactivating();
}

void AReactor::FinishDeactivating()
{
	if(_bDeactivating == 1)
	{
		_bDeactivating = 2;
	}
	else if(_bDeactivating == 2)
	{
		SetupByState(false, EReactorStateSource::RSS_Deactivating);
		TArray<UReactionComponent*> reactions;
		GetComponents(reactions, false);
		for(auto& reaction:reactions)
		{
			if(reaction->GetReactionEnabled())
			{
				reaction->OnReactorSetupByState(false, EReactorStateSource::RSS_Deactivating);
			}
		}
		_bDeactivating = 0;
	}
}

void AReactor::DeactivationFailed_Implementation(AController* controller)
{

}

void AReactor::SetupByState_Implementation(bool Activated, EReactorStateSource stateSource)
{

}

void AReactor::SaveInitialState_Implementation()
{
	//Calls my own StoreState with InitialState
	Execute_StoreState(this, Execute_GetInitialStateKeeper(this), ESaveStateStoreOperation::SSSO_SaveInitialState);
	
	//Call reaction components so that they have the chance to store their data in our InitialState
	TArray<UReactionComponent*> reactions;
	GetComponents(reactions, false);
	for(auto& reaction:reactions)
	{
		reaction->OnReactorStoreState(Execute_GetInitialStateKeeper(this),ESaveStateStoreOperation::SSSO_SaveInitialState);
	}
}

void AReactor::SaveState_Implementation()
{
	//Calls my own StoreState with SavedState
	Execute_StoreState(this, Execute_GetSavedStateKeeper(this), ESaveStateStoreOperation::SSSO_SaveState);
	
	//Call reaction components so that they have the chance to store their data in our SavedState
	TArray<UReactionComponent*> reactions;
	GetComponents(reactions, false);
	for(auto& reaction:reactions)
	{
		reaction->OnReactorStoreState(Execute_GetSavedStateKeeper(this),ESaveStateStoreOperation::SSSO_SaveState);
	}
}

void AReactor::RestartToInitialState_Implementation()
{	
	//Calls my own SetupByParameters with InitialState
	Execute_SetupByParameters(this, Execute_GetInitialStateKeeper(this), ESaveStateSetupOperation::SSSO_Restarting);
	
	//Call reaction components so that they have the chance to retrieve their data from our InitialState
	TArray<UReactionComponent*> reactions;
	GetComponents(reactions, false);
	for(auto& reaction:reactions)
	{
		reaction->OnReactorSetupByParameters(Execute_GetInitialStateKeeper(this),ESaveStateSetupOperation::SSSO_Restarting);
	}

	SetupByState(_bActivated, EReactorStateSource::RSS_Restarting);

	for(const auto& reaction:reactions)
	{
		reaction->OnReactorSetupByState(IsActivated(), EReactorStateSource::RSS_Restarting);
	}
}

void AReactor::LoadState_Implementation()
{
	//Calls my own SetupByParameters with SavedState
	Execute_SetupByParameters(this, Execute_GetSavedStateKeeper(this), ESaveStateSetupOperation::SSSO_Loading);
	
	//Call reaction components so that they have the chance to retrieve their data from our SavedState
	TArray<UReactionComponent*> reactions;
	GetComponents(reactions, false);
	for(const auto& reaction:reactions)
	{
		reaction->OnReactorSetupByParameters(Execute_GetSavedStateKeeper(this),ESaveStateSetupOperation::SSSO_Loading);
	}

	SetupByState(_bActivated, EReactorStateSource::RSS_Loading);

	for(const auto& reaction:reactions)
	{
		reaction->OnReactorSetupByState(IsActivated(), EReactorStateSource::RSS_Loading);
	}

	SavedState->ConsumeOverrideData();
}

void AReactor::StoreState_Implementation(UParameterKeeper* ParameterKeeper, ESaveStateStoreOperation Operation)
{
	if(ParameterKeeper)
	{
		ParameterKeeper->SetBool("Activated", IsActivated());
		ParameterKeeper->SetBool("EverActivated", WasEverActivated());
	}
}

void AReactor::SetupByParameters_Implementation(UParameterKeeper* ParameterKeeper, ESaveStateSetupOperation Operation)
{
	if(ParameterKeeper)
	{
		_bActivated = ParameterKeeper->GetBool("Activated");
		_bEverActivated = ParameterKeeper->GetBool("EverActivated");
	}
}

void AReactor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if(bAutoDeactivate)
	{
		bIgnoreDetectorsForDeactivation = false;
		bPreventInteractionDeactivation = true;
	}
	if(bAutoActivate)
	{
		bPreventInteractionActivation = true;
	}
}

