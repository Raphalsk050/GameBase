// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase/Public/Core/Character/CharacterBase.h"

#include "GameBase/Public/Core/PlayerState/PlayerStateBase.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APlayerStateBase* PS = GetPlayerState<APlayerStateBase>();
	if (PS->IsValidLowLevel())
	{
		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
}

UAbilitySystemComponentBase* ACharacterBase::GetAbilitySystemComponent() const
{
	return GetPlayerState<APlayerStateBase>()->GetAbilitySystemComponent();
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


//OnRep functions
void ACharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	APlayerStateBase* PS = GetPlayerState<APlayerStateBase>();
	if (PS)
	{
		// Set the ASC for clients. Server does this in PossessedBy.
		const auto AbilitySystemComponent = PS->GetAbilitySystemComponent();

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	}
}
// ...

