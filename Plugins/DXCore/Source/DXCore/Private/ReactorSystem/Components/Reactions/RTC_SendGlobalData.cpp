// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorSystem/Components/Reactions/RTC_SendGlobalData.h"
#include "Core/DXCoreGISubsystem.h"

void URTC_SendGlobalData::OnReactorActivating_Implementation(AController* controller)
{
	const UWorld* world = GetOwner()->GetWorld();

	if(IsValid(world))
	{
		UDXCoreGISubsystem* coreGISubsystem = world->GetGameInstance()->GetSubsystem<UDXCoreGISubsystem>();

		for (FIntGlobalData update : IntRequirementUpdates)
		{
			coreGISubsystem->SendRequiredIntByStruct(update);
		}
		
		for (FFloatGlobalData update : FloatRequirementUpdates)
		{
			coreGISubsystem->SendRequiredFloatByStruct(update);
		}
		
		for (FTextGlobalData update : TextRequirementUpdates)
		{
			coreGISubsystem->SendRequiredStringByStruct(update);
		}
		
		for (FBoolGlobalData update : BoolRequirementUpdates)
		{
			coreGISubsystem->SendRequiredBoolByStruct(update);
		}
	}
}