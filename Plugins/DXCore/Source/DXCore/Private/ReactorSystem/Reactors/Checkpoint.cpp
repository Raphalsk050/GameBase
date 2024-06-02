// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorSystem/Reactors/Checkpoint.h"


#include "EngineUtils.h"
#include "Core/DXWorldSettings.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACheckpoint::ACheckpoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneComponent;

	ChildPlayerStart = CreateDefaultSubobject<UChildActorComponent>("ChildPlayerStart");
	if(ChildPlayerStart)
	{
		ChildPlayerStart->SetChildActorClass(APlayerStart::StaticClass());
		ChildPlayerStart->SetupAttachment(RootComponent);
		//ChildPlayerStart->RegisterComponent();
	}
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACheckpoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	TArray<AActor*> allCheckpoints;
	UGameplayStatics::GetAllActorsOfClass(this, ACheckpoint::GetClass(), allCheckpoints);
	for(AActor* checkpointActor : allCheckpoints)
	{
		if(checkpointActor != this)
		{
			ACheckpoint* otherCheckpoint = Cast<ACheckpoint>(checkpointActor);
			if(bInitialBuild)
			{
				otherCheckpoint->bInitialBuild = false;
			}
			if(bInitialDev)
			{
				otherCheckpoint->bInitialDev = false;
			}
			if(Identifier == otherCheckpoint->Identifier)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
                    FString::Printf(TEXT("%s Identifier is the same as: %s's Identifier. Please change one of them before playing!"),
                        *UKismetSystemLibrary::GetDisplayName(this),
                        *UKismetSystemLibrary::GetDisplayName(otherCheckpoint)));
			}
		}
	}
}

void ACheckpoint::Activating_Implementation(AController* controller)
{
	Super::Activating_Implementation(controller);
	if (UWorld* world = GetWorld())
	{
		ADXWorldSettings* ws = Cast<ADXWorldSettings>(world->GetWorldSettings());
		if(ws->GetCurrentCheckpointOrder() <= Order)
		{
			ws->SetCurrentCheckpoint(this);

			for (FActorIterator It(world); It; ++It)
			{
				AActor* Actor = *It;
				if (Actor->GetClass()->ImplementsInterface(USaveStateInterface::StaticClass()))
				{
					const auto* interfaceActor = Cast<ISaveStateInterface>(Actor);
					interfaceActor->Execute_SaveState(Actor);
				}
			}	
		}
	}
}

