// Fill out your copyright notice in the Description page of Project Settings.


#include "RequirementSystem/Requirable.h"


// Add default functionality here for any IRequirable functions that are not pure virtual.
bool IRequirable::IsRequirementMet_Implementation(FRequirement requirement) const
{
	const AActor* myself = Cast<AActor>(this);
	if (myself)
	{
		return requirement.CheckRequirement(myself);
	}
	return false;
}

void IRequirable::ReceiveGlobalRequirementConcluded_Implementation(FRequirement requirementValue)
{
}

void IRequirable::ReceiveGlobalRequirementChanged_Implementation(FRequirement requirementValue, bool isRequirementMet)
{
}
