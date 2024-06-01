// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Controller/FollowCamera.h"

// Sets default values
AFollowCamera::AFollowCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    AActor::SetActorTickEnabled(false);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	RootComponent = SpringArm;
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AFollowCamera::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFollowCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Follow(DeltaTime);
}

void AFollowCamera::Initialize()
{
	SpringArm->TargetArmLength = CameraInfo->TargetArmLength;
	SpringArm->SocketOffset = CameraInfo->TargetLocationOffset;
	SpringArm->bDoCollisionTest = CameraInfo->bDoCollisionTest;
	Camera->FieldOfView = CameraInfo->CameraFieldOfView;
	AActor::SetActorTickEnabled(true);
}

void AFollowCamera::Follow(float DeltaTime)
{
	if(!FollowTarget->IsValidLowLevel()) return;
	auto CurrentLocation = GetActorLocation();
	auto TargetLocation = FollowTarget->GetActorLocation();
	auto CharacterVelocity = FollowTarget->GetVelocity();
	SetActorRotation(FollowTarget->GetInstigatorController()->GetControlRotation());
	auto ChangeLengthDecay = CameraInfo->SpeedPassToChangeTargetArmLenght;
	auto MovementSpeed = CameraInfo->SpeedPassToFollow;
	
	if(CharacterVelocity.Length() > 0.1f)
	{
		SpringArm->TargetArmLength = ExpDecay(SpringArm->TargetArmLength, CameraInfo->CombatAndMovementTargetArmLength, ChangeLengthDecay, DeltaTime);
	}

	else
	{
		SpringArm->TargetArmLength = ExpDecay(SpringArm->TargetArmLength, CameraInfo->TargetArmLength, ChangeLengthDecay, DeltaTime);
	}
	
	//Alternative one
	//FMath::ExponentialSmoothingApprox(CurrentLocation,TargetLocation, FollowPassing, SmoothingTime);

	//Alternative two
	CurrentLocation = ExpDecay(CurrentLocation, TargetLocation, MovementSpeed, DeltaTime);
	SetActorLocation(CurrentLocation);
}

