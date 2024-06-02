// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DetectionComponent.h"
#include "GameFramework/Actor.h"
#include "SaveSystem/SaveStateInterface.h"
#include "Reactor.generated.h"

class UKeyLocatorComponent;
UENUM(BlueprintType)
enum class EDetectionConfig : uint8
{
	DC_MustDetectTrue	UMETA(DisplayName = "Must Detect True"),
	DC_MustDetectFalse	UMETA(DisplayName = "Must Detect False")
};

UENUM(BlueprintType) 
enum class EDetectionConfigRelation : uint8
{
	DCR_And	UMETA(DisplayName = "AND"),
	DCR_Or UMETA(DisplayName = "OR Activation")
};

UCLASS(BlueprintType, Blueprintable)
class DXCORE_API AReactor : public AActor, public ISaveStateInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReactor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	bool bCanRepeat = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	bool bCanDeactivate = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	bool bInteractable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	FName InteractTag;
	
	//---Activation

	/*List of Detection components that will be taken into account for activation. 
	You must type the component's name and its desired configuration for activation.
	When Empty, it takes every Detection component into account for activation.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Activation", DisplayName= "Filtered Activation")
	TMap<FName, EDetectionConfig> ActivationDetectionOverride;
	
	//If true, once the configured detection is reached by a controller, the Reactor will try to Activate itself with that controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Activation")
	bool bAutoActivate;
	
	//If true, once the configured detection is reached by a controller, the Reactor will enable input on itself for that controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Activation")
	bool bAutoEnableInput;

	//If true, the CanInteract function will return false whenever the reactor is Deactivated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Activation", meta=(EditCondition="!bAutoActivate"))
	bool bPreventInteractionActivation;

	/*AND: every DetectionConfig is needed for activate/deactivate
	OR: only one DetectionConfig is needed for activate/deactivate.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Activation")
	EDetectionConfigRelation ActivationConfigRelation;

	
	//---Deactivation

	/*List of Detection components that will be taken into account for activation. 
	You must type the component's name and its desired configuration for activation.
	When Empty, it takes every DetectionComponent into account for deactivation.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Deactivation", DisplayName= "Filtered Deactivation")
	TMap<FName, EDetectionConfig> DeactivationDetectionOverride;
	
	//If true, once the configured detection is reached by a controller, the Reactor will try to Deactivate itself with that controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Deactivation")
	bool bAutoDeactivate;
	
	//If true, once the configured detection is reached by a controller, the Reactor will disable input on itself for that controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Deactivation")
	bool bAutoDisableInput;

	//If true, the CanInteract function will return false whenever the reactor is Activated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Deactivation", meta=(EditCondition="!bAutoDeactivate"))
	bool bPreventInteractionDeactivation = true;

	//If true, it will ignore all detectors when trying to Deactivate (free to disable)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Deactivation", meta=(EditCondition="!bAutoDeactivate"))
	bool bIgnoreDetectorsForDeactivation;

	/*AND: every DetectionConfig is needed for activate/deactivate
	OR: only one DetectionConfig is needed for activate/deactivate.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default|Deactivation")
	EDetectionConfigRelation DeactivationConfigRelation;
	
private:
	bool _bActivated;
	bool _bEverActivated;

	bool _bActivationDetectionInfiniteLoopFlag;
	bool _bDetectingActivationInfiniteLoopFlag;

	//0 for not activating, 1 for still running activating, 2 for have run activating on reactions
	uint8 _bActivating;
	//0 for not deactivating, 1 for still running deactivating, 2 for have run deactivating on reactions
	uint8 _bDeactivating;

	TArray<UKeyLocatorComponent*> keyLocatorComponents;

	UPROPERTY()
	TMap<UDetectionComponent*, EDetectionConfig> activationConfigs;
	UPROPERTY()
	TMap<UDetectionComponent*, EDetectionConfig> deactivationConfigs;
	
	UPROPERTY()
	AController* _controllerThatActivatedLast;
	UPROPERTY()
	AController* _controllerThatDeactivatedLast;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="DXCore|Reactor System")
	bool Detect(AController* controller);
	UFUNCTION(BlueprintCallable, Category="DXCore|Reactor System")
	bool Activate(AController* controller);
	UFUNCTION(BlueprintCallable, Category="DXCore|Reactor System")
	bool Deactivate(AController* controller);

	UFUNCTION(BlueprintPure)
	TArray<UKeyLocatorComponent*> GetActiveKeyLocators();

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool CanInteract();
	virtual bool CanInteract_Implementation();

	UFUNCTION(BlueprintPure, Category="DXCore|Reactor System")
	bool IsActivated() const;
	UFUNCTION(BlueprintPure, Category="DXCore|Reactor System")
	bool WasEverActivated() const;

protected:
	
	UFUNCTION(BlueprintNativeEvent, Category="DXCore|Reactor System")
	void Activating(AController* controller);
	virtual void Activating_Implementation(AController* controller);
	UFUNCTION(BlueprintCallable, Category="DXCore|Reactor System")
	void FinishActivating();

	UFUNCTION(BlueprintNativeEvent, Category="DXCore|Reactor System")
	void ActivationFailed(AController* controller);
	virtual void ActivationFailed_Implementation(AController* controller);

	UFUNCTION(BlueprintNativeEvent, Category="DXCore|Reactor System")
	void Deactivating(AController* controller);
	virtual void Deactivating_Implementation(AController* controller);
	UFUNCTION(BlueprintCallable, Category="DXCore|Reactor System")
	void FinishDeactivating();

	UFUNCTION(BlueprintNativeEvent, Category="DXCore|Reactor System")
	void DeactivationFailed(AController* controller);
	virtual void DeactivationFailed_Implementation(AController* controller);

	//True if it was activating and false if it was deactivating
	UFUNCTION(BlueprintNativeEvent, Category="DXCore|Reactor System")
	void SetupByState(bool Activated, EReactorStateSource stateSource);
	virtual void SetupByState_Implementation(bool Activated, EReactorStateSource stateSource);

	//======= Save System
protected:	
	GENERATE_SAVESTATE_BODY_CustomSaveLoad();

	//---- Functions to override and store states
	
	virtual void StoreState_Implementation(UParameterKeeper* ParameterKeeper, ESaveStateStoreOperation Operation) override;
	virtual void SetupByParameters_Implementation(UParameterKeeper* ParameterKeeper, ESaveStateSetupOperation Operation) override;
public:
	virtual void OnConstruction(const FTransform& Transform) override;

	//---- Custom functionality for boilet plate functions
	
	virtual void SaveInitialState_Implementation() override;
	virtual void SaveState_Implementation() override;
	virtual void RestartToInitialState_Implementation() override;
	virtual void LoadState_Implementation() override;

	//=======
};
