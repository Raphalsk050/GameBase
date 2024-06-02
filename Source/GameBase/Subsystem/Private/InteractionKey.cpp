// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase/Subsystem/Public/InteractionKey.h"

bool FInteractionKey::CheckKey() const
{
	return DetectionType == EDetectionType::MustDetectTrue ? bCurrentValue: !bCurrentValue;
}