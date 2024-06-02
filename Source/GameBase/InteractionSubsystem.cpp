// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase\InteractionSubsystem.h"

#include "Subsystem/Public/RequiredComponent.h"


void UInteractionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// Initialization logic here
	UE_LOG(LogTemp, Warning, TEXT("InteractionSubsystem Initialized"));
}

void UInteractionSubsystem::Deinitialize()
{
	// Cleanup logic here
	UE_LOG(LogTemp, Warning, TEXT("InteractionSubsystem Deinitialized"));
	Super::Deinitialize();
}

bool UInteractionSubsystem::RegisterKey(const FString KeyName, const bool InitialValue, const EDetectionType DetectionType)
{
	//if this keys already registered, ignore
	for (auto InteractionKey : InteractionKeys)
	{
		if(InteractionKey.KeyName == KeyName)
		{
			return false;
		}
	}
	
	auto NewKey = FInteractionKey();
	NewKey.KeyName = KeyName;
	NewKey.DetectionType = DetectionType;
	NewKey.bCurrentValue = InitialValue;
	InteractionKeys.Add(NewKey);
	UE_LOG(LogTemp, Warning, TEXT("%s key was registered!"), *KeyName);
	return true;
}

void UInteractionSubsystem::SendGlobalRequirement(const FString KeyName, const bool Value)
{
	for(int i = 0; i < InteractionKeys.Num(); i++)
	{
		if(InteractionKeys[i].KeyName == KeyName)
		{
			InteractionKeys[i].bCurrentValue = Value;
			CheckAndTriggerKeys();
			return;
		}
	}
}

void UInteractionSubsystem::UnregisterKey(FString Key)
{
	for(int i = 0; i < InteractionKeys.Num(); i++)
	{
		if(InteractionKeys[i].KeyName == Key)
		{
			InteractionKeys.RemoveAt(i);
		}
	}
}

void UInteractionSubsystem::CheckAndTriggerKeys()
{
	for (auto Key : InteractionKeys)
	{
		if (Key.CheckKey())
		{
			NotifyObservers(Key.KeyName);
		}
	}
}

//We need both the register observer and the register key to behave correctly
//if a key has been triggered before the observer was registered.
void UInteractionSubsystem::RegisterObserver(const FString KeyName, const bool InitialValue, const EDetectionType DetectionType, IObserver* Observer)
{
	RegisterKey(KeyName, InitialValue, DetectionType);
	
	Observers.FindOrAdd(KeyName).Add(Observer);
	
	//After being registered, the observers must be notified if the
	//related keys have already been activated
	CheckAndTriggerKeys();

	UE_LOG(LogTemp, Warning, TEXT("%s Observer was registered"), *Cast<URequiredComponent>(Observer)->GetOwner()->GetName());
}

void UInteractionSubsystem::UnregisterObserver(FString Key, IObserver* Observer)
{
	if (Observers.Contains(Key))
	{
		Observers[Key].Remove(Observer);
	}
}

void UInteractionSubsystem::NotifyObservers(FString Key)
{
	if (Observers.Contains(Key))
	{
		for (IObserver* Observer : Observers[Key])
		{
			Observer->OnNotify(Key);
		}
	}
}
