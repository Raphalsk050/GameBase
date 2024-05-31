// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Controller/FollowCamera.h"

// Sets default values
AFollowCamera::AFollowCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	SpringArm->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFollowCamera::BeginPlay()
{
	Super::BeginPlay();
	SpringArm->TargetArmLength = CameraInfo->TargetArmLength;
	SpringArm->TargetOffset = CameraInfo->TargetLocationOffset;
	Camera->FieldOfView = CameraInfo->CameraFieldOfView;
}

// Called every frame
void AFollowCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Follow(DeltaTime,MovementDecay);
}

void AFollowCamera::Follow(float FollowPassing, float SmoothingTime)
{
	auto CurrentLocation = GetActorLocation();
	auto TargetLocation = FollowTarget->GetActorLocation();

	//Alternative one
	//FMath::ExponentialSmoothingApprox(CurrentLocation,TargetLocation, FollowPassing, SmoothingTime);

	//Alternative two
	CurrentLocation = ExpDecay(CurrentLocation, TargetLocation, SmoothingTime, FollowPassing);
	SetActorLocation(CurrentLocation);
}

FVector AFollowCamera::ExpDecay(FVector A, FVector B, int Decay, float DeltaTime)
{
	return B+(A-B)*FMath::Exp(-Decay*DeltaTime);
}

