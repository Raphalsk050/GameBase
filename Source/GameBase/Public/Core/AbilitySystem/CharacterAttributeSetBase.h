// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "UObject/CoreNet.h"
#include "Net/UnrealNetwork.h"
#include "CharacterAttributeSetBase.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class GAMEBASE_API UCharacterAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "CharacterLevel")
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, CharacterLevel)
	
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Health)

	UPROPERTY(BlueprintReadOnly, Category = "MaxHealth")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MaxWalkSpeed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxWalkSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSetBase, MaxStamina)
};
