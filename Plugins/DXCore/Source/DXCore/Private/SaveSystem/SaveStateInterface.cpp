// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/SaveStateInterface.h"


UParameterKeeper* ISaveStateInterface::GetInitialStateKeeper_Implementation()
{
	return nullptr;
}

UParameterKeeper* ISaveStateInterface::GetSavedStateKeeper_Implementation()
{
	return nullptr;
}

void ISaveStateInterface::StoreState_Implementation(UParameterKeeper* ParameterKeeper,
	ESaveStateStoreOperation Operation)
{
}

void ISaveStateInterface::SetupByParameters_Implementation(UParameterKeeper* ParameterKeeper,
	ESaveStateSetupOperation Operation)
{
}

void ISaveStateInterface::SaveInitialState_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("If you reached here you probably should have used one of the Macros in the Header or overridden"));
	StoreState(GetInitialStateKeeper(), ESaveStateStoreOperation::SSSO_SaveInitialState);
}

void ISaveStateInterface::SaveState_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("If you reached here you probably should have used one of the Macros in the Header or overridden"));
	StoreState(GetSavedStateKeeper(), ESaveStateStoreOperation::SSSO_SaveState);
}

void ISaveStateInterface::RestartToInitialState_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("If you reached here you probably should have used one of the Macros in the Header or overridden"));
	SetupByParameters(GetInitialStateKeeper(), ESaveStateSetupOperation::SSSO_Restarting);
}

void ISaveStateInterface::LoadState_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("If you reached here you probably should have used one of the Macros in the Header or overridden"));
	SetupByParameters(GetSavedStateKeeper(), ESaveStateSetupOperation::SSSO_Loading);
}

// Add default functionality here for any ISaveStateInterface functions that are not pure virtual.
void ISaveStateInterface::ImportSavedState_Implementation(FParameterKeeperData importData)
{
	UE_LOG(LogTemp, Log, TEXT("If you reached here you probably should have used one of the Macros in the Header or overridden"));
	GetSavedStateKeeper()->ImportData(importData);
}

FParameterKeeperData ISaveStateInterface::ExportSavedStateData_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("If you reached here you probably should have used one of the Macros in the Header or overridden"));
	return GetSavedStateKeeper()->ExportParameterData();
}

void ISaveStateInterface::OverrideNextSavedState_Implementation(FParameterKeeperData overrideData)
{
	UE_LOG(LogTemp, Log, TEXT("If you reached here you probably should have used one of the Macros in the Header or overridden"));
	GetSavedStateKeeper()->OverrideData(overrideData);
}
