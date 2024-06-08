// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBase/Public/Core/Controller/PlayerControllerBase.h"

#include "Core/PlayerState/PlayerStateBase.h"

APlayerControllerBase::APlayerControllerBase()
{
}

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const AActor* ControlledActor = GetPawn();
	APlayerStateBase* PlayerStateBase = Cast<APlayerStateBase>(GetPawn()->GetPlayerState());
	if (IsValid(ControlledActor) && ControlledActor->InputComponent) {
		PlayerStateBase->GetAbilitySystemComponentBase()->SetInputComponent(Cast<UEnhancedInputComponent>(ControlledActor->InputComponent));
	}
	
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
			if(auto AbilitySystemComponent = Cast<APlayerStateBase>(InPawn->GetPlayerState<APlayerStateBase>())->GetAbilitySystemComponentBase())
			{
				auto AbilitySpecHandle = AbilitySystemComponent->GiveAbility(AbilitySpec);
				AbilitySystemComponent->SetInputBinding(InputBind,AbilitySpecHandle);
			}
		}
	});

	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);

	//The character camera
	if(!FollowCamera->IsValidLowLevel())
	{
		FollowCamera = GetWorld()->SpawnActor<AFollowCamera>(FVector(0.0f),FRotator(0.0f));
		
	}
	SetViewTargetWithBlend(FollowCamera);
	FollowCamera->FollowTarget = GetPawn();
	FollowCamera->CameraInfo = CameraInfo;
	FollowCamera->Initialize();
}

void APlayerControllerBase::OnUnPossess()
{
	FollowCamera->Deinitialize();
	Super::OnUnPossess();
}
