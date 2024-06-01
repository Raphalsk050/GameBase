// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Data/CameraInfo.h"
#include "FollowCamera.generated.h"

UCLASS()
class GAMEBASE_API AFollowCamera : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFollowCamera();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCameraInfo* CameraInfo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(HideInDetailPanel))
	AActor* FollowTarget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCameraComponent* Camera;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Initialize();

private:
	void Follow(float DeltaTime);

	template<typename T>
	auto ExpDecay(const T& A,const T& B, int Decay, float DeltaTime) -> decltype(B+(A-B)*FMath::Exp(-Decay*DeltaTime))
	{
		return B+(A-B)*FMath::Exp(-Decay*DeltaTime);
	}
};
