// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InteractionKey.generated.h"

UENUM(BlueprintType)
enum class EKeyType : uint8
{
	Bool,
	String
};

UENUM(BlueprintType)
enum class EDetectionType : uint8
{
	MustDetectTrue,
	MustDetectFalse
};

USTRUCT(Blueprintable)
struct GAMEBASE_API FInteractionKey
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString KeyName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EDetectionType DetectionType;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCurrentValue = false;
	
	/*#if WITH_EDITOR
		UPROPERTY(BlueprintReadWrite, meta=(EditCondition = "!bIsString"))
		bool bIsBoolean = true;
	
		UPROPERTY(BlueprintReadWrite, meta=(EditCondition = "!bIsBoolean"))
		bool bIsString = false;
	#endif*/
	
	bool CheckKey() const;
};