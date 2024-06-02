// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Core/DXCoreGISubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BaseActiveUserSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DXCORE_API UBaseActiveUserSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	
	UDXCoreGISubsystem* _giSubCache = nullptr;
	UDXCoreGISubsystem* GetGISub();

public:
	FTSTicker::FDelegateHandle TickerDelegateHandle;
	bool TickDelegate(float DeltaTime);
};
