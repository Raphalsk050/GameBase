// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DXWorldSettings.h"
#include "ReactorSystem/Reactors/Checkpoint.h"

ELevelPlayState ADXWorldSettings::GetLevelPlayState()
{
	return LevelPlayState;
}

void ADXWorldSettings::SetCurrentCheckpoint(ACheckpoint* newCheckpoint)
{
	if (newCheckpoint == nullptr)
	{
		_currentCheckpoint = "NOCHECKPOINT";
		_currentCheckpointOrder = 0;
	}
	else
	{
		_currentCheckpoint = newCheckpoint->Identifier;
		_currentCheckpointOrder = newCheckpoint->Order;
	}
	_currentCheckpointActor = newCheckpoint;
}
