// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorSystem/Components/Reactions/RTC_ChainReaction.h"

#include "ReactorSystem/Reactor.h"

void URTC_ChainReaction::OnReactorActivating_Implementation(AController* controller)
{
	for(auto& reactor : Reactors)
	{
		reactor->Activate(controller);
	}
}