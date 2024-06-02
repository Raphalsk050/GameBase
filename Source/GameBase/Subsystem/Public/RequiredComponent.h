// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionKey.h"
#include "Components/ActorComponent.h"
#include "GameBase/InteractionSubsystem.h"
#include "RequiredComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSuccessKeyNotify);

UCLASS(Blueprintable, BlueprintType,ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class GAMEBASE_API URequiredComponent : public UActorComponent, public IObserver
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URequiredComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInteractionKey> InteractionKey;

	UPROPERTY(BlueprintAssignable)
	FOnSuccessKeyNotify NotifyDelegate;
	
	virtual void OnNotify(FString Key) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnNotifyInternal();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
