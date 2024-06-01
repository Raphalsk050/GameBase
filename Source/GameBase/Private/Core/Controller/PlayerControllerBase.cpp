// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase/Public/Core/Controller/PlayerControllerBase.h"

#include "Core/PlayerState/PlayerStateBase.h"

APlayerControllerBase::APlayerControllerBase()
{
}

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
	const AActor* ControlledActor = GetPawn();
	APlayerStateBase* PlayerStateBase = Cast<APlayerStateBase>(GetPawn()->GetPlayerState());
	if (IsValid(ControlledActor) && ControlledActor->InputComponent) {
		PlayerStateBase->GetAbilitySystemComponent()->SetInputComponent(Cast<UEnhancedInputComponent>(ControlledActor->InputComponent));
	}
}

void APlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(!CharacterDefaultAbilities->IsValidLowLevel()) return;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this, InPawn]()
	{
		auto DefaultAbilityInfos = CharacterDefaultAbilities->Abilities;
	
		for(auto DefaultAbility : DefaultAbilityInfos)
		{
			auto Ability = DefaultAbility.Key;
			auto InputBind = DefaultAbility.Value;
			auto AbilitySpec = FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->AbilityInputID), this);
			auto AbilitySystemComponent = Cast<APlayerStateBase>(InPawn->GetPlayerState())->GetAbilitySystemComponent();
			auto AbilitySpecHandle = AbilitySystemComponent->GiveAbility(AbilitySpec);
			AbilitySystemComponent->SetInputBinding(InputBind,AbilitySpecHandle);
		}
	});

	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);

	//The character camera
	FollowCamera = GetWorld()->SpawnActor<AFollowCamera>(FVector(0.0f),FRotator(0.0f));
	FollowCamera->FollowTarget = GetPawn();
	FollowCamera->CameraInfo = CameraInfo;
	SetViewTargetWithBlend(FollowCamera);
	FollowCamera->Initialize();
}
