// Fill out your copyright notice in the Description page of Project Settings.


#include "Platforms/BaseActiveUserSubsystem.h"
#include "GameFramework/PlayerInput.h"
#include "Engine/GameInstance.h"

void UBaseActiveUserSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	//TODO: BaseActiveUser macro can't be found in switch builds; macros should also use full uppercase
//#if BaseActiveUser || WITH_EDITOR
	FTickerDelegate TickerDelegate = FTickerDelegate::CreateUObject(this, &UBaseActiveUserSubsystem::TickDelegate);
	TickerDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickerDelegate);
//#endif
}

UDXCoreGISubsystem* UBaseActiveUserSubsystem::GetGISub()
{
	if(_giSubCache == nullptr)
	{
		_giSubCache = Cast<UDXCoreGISubsystem>(GetGameInstance()->GetSubsystem<UDXCoreGISubsystem>());
	}
	return _giSubCache;
}

bool UBaseActiveUserSubsystem::TickDelegate(float DeltaTime)
{
	if (!GetGISub()->bCanLookForFirstPlayerInput)
	{
		return true;
	}

	UGameInstance* gi = GetGameInstance();
	if (gi->GetFirstGamePlayer() && gi->GetFirstGamePlayer()->PlayerController && gi->GetFirstGamePlayer()->PlayerController->PlayerInput->WasJustPressed(EKeys::AnyKey))
	{
		GetGISub()->SetActiveLocalPlayer(gi->GetFirstGamePlayer());
		//gi->lastActivePlayer = gi->activeLocalPlayer; -- Seemed unnecessary. Could just set again to make sure, now that lastActive is protected.
		GetGISub()->OnUserSet.Broadcast();
		return false;
	}
	return true;
}
