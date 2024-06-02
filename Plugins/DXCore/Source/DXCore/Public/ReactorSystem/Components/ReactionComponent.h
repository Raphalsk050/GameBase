// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DXCoreImportantTypes.h"
#include "ReactionComponent.generated.h"

enum class ESaveStateStoreOperation : uint8;
enum class ESaveStateSetupOperation : uint8;
class AReactor;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DXCORE_API UReactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UReactionComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReactionDelay = 0.0f;
	
	UFUNCTION(BlueprintPure, Category = "Reactor|ReactionComponent")
	bool GetReactionEnabled();
	
	UFUNCTION(BlueprintCallable, Category = "Reactor|ReactionComponent")
	void SetReactionEnabled(bool newValue);

private:
	AReactor* reactorOwner;

	FTimerHandle delayTimerHandle;
protected:

	//Shouldn't be changed runtime for now
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reactor|ReactionComponent", meta = (DisplayName = "Reaction Enabled"))
	uint8 _bReactionEnabled : 1;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Reactor|ReactionComponent")
	void OnReactionEnabled();
	virtual void OnReactionEnabled_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Reactor|ReactionComponent")
	void OnReactionDisabled();
	virtual void OnReactionDisabled_Implementation();
public:
	//External Call on Reactors to run Activation
	void StartReacting(AController* controller);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, Category = "DXCore|Reactor System|ReactionComponent")
	void OnReactorBeginPlay();
	virtual void OnReactorBeginPlay_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, Category = "DXCore|Reactor System|ReactionComponent")
	void OnReactorActivating(AController* controller);
	virtual void OnReactorActivating_Implementation(AController* controller);

	UFUNCTION(BlueprintNativeEvent, Category = "DXCore|Reactor System|ReactionComponent")
	void OnReactorActivationFailed(AController* controller);
	virtual void OnReactorActivationFailed_Implementation(AController* controller);

	UFUNCTION(BlueprintNativeEvent, Category = "DXCore|Reactor System|ReactionComponent")
	void OnReactorDeactivating(AController* controller);
	virtual void OnReactorDeactivating_Implementation(AController* controller);

	UFUNCTION(BlueprintNativeEvent, Category = "DXCore|Reactor System|ReactionComponent")
	void OnReactorDeactivationFailed(AController* controller);
	virtual void OnReactorDeactivationFailed_Implementation(AController* controller);

	UFUNCTION(BlueprintNativeEvent, Category = "DXCore|Reactor System|ReactionComponent")
	void OnReactorSetupByState(bool Activated, EReactorStateSource stateSource);
	virtual void OnReactorSetupByState_Implementation(bool Activated, EReactorStateSource stateSource);

	//----- Save State System Functions

	//Use only as a means to save stuff on the Reactor (ParameterKeeper), never store states in the reaction components itself
	UFUNCTION(BlueprintNativeEvent, Category = "DXCore|Reactor System|ReactionComponent")
	void OnReactorStoreState(UParameterKeeper* ParameterKeeper, ESaveStateStoreOperation Operation);
	virtual void OnReactorStoreState_Implementation(UParameterKeeper* ParameterKeeper, ESaveStateStoreOperation Operation);

	//Use only as a means to load stuff by getting info from the Reactor (ParameterKeeper), never use states from the reaction components itself
	UFUNCTION(BlueprintNativeEvent, Category = "DXCore|Reactor System|ReactionComponent")
	void OnReactorSetupByParameters(UParameterKeeper* ParameterKeeper, ESaveStateSetupOperation Operation);
	virtual void OnReactorSetupByParameters_Implementation(UParameterKeeper* ParameterKeeper, ESaveStateSetupOperation Operation);
protected:
	virtual void OnRegister() override;

	UFUNCTION(BlueprintPure, Category = "DXCore|Reactor System|ReactionComponent")
	AReactor* GetReactorOwner() const { return reactorOwner; }
public:
	virtual void InitializeComponent() override;
};
