// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorSystem/Components/Detections/DTC_LookAt.h"

#include "Kismet/KismetMathLibrary.h"

UDTC_LookAt::UDTC_LookAt()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickInterval(0.2f);
}

bool UDTC_LookAt::DetectInternal_Implementation(AController* controller)
{
	const FVector _location = this->GetComponentLocation();
	return IsPositionInsideScreenRadius(_location,ScreenPercentTolerance);
}

bool UDTC_LookAt::IsPositionInsideScreenRadius(FVector Position, float ScreenTolerance)
{
	//Get Distance to the center of the screen
	int32 viewportSizeX;
	int32 viewportSizeY;
	APlayerController* _controller = GetWorld()->GetFirstPlayerController();
	_controller->GetViewportSize(viewportSizeX, viewportSizeY);
	const FVector2D viewportSize = FVector2D(viewportSizeX, viewportSizeY);

	FVector2D screenLocation;
	_controller->ProjectWorldLocationToScreen(Position, screenLocation);
	screenLocation = screenLocation - (viewportSize / 2.0);
			
	float Distance = UKismetMathLibrary::VSizeXY(UKismetMathLibrary::Conv_Vector2DToVector(screenLocation));

	//Convert Distance in Percentage
	float DistanceInPercentage = (Distance / (viewportSize.X / 2.0)) * 100;

	//Compare Distance with Tolerance
	return DistanceInPercentage < ScreenTolerance;
}

