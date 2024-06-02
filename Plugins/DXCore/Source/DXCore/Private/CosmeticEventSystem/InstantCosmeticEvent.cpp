// Fill out your copyright notice in the Description page of Project Settings.


#include "CosmeticEventSystem/InstantCosmeticEvent.h"

UInstantCosmeticEvent::UInstantCosmeticEvent()
{
	
}

bool UInstantCosmeticEvent::Execute_Implementation(FCosmeticEventData ParameterKeeperData, const UObject* WorldContextObject)
{
	return true;
}

UWorld* UInstantCosmeticEvent::GetWorld() const
{
	return WorldContext;
}
