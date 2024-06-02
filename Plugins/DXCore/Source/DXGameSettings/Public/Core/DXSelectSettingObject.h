// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/SettingObject.h"
#include "DXSelectSettingObject.generated.h"

/**
 * A SettingObject that has a list of options to choose from
 */
UCLASS()
class DXGAMESETTINGS_API UDXSelectSettingObject : public USettingObject
{
	GENERATED_BODY()
	
public:
	UDXSelectSettingObject(const FObjectInitializer& ObjectInitializer);
	
	virtual void InitSetting_Implementation(UObject* WorldContextObject) override;

	UFUNCTION(BlueprintCallable)
	void ChangeSelectIndex(int Index);
	UFUNCTION(BlueprintCallable)
	void ChangeSelectOption(const FString& Option);

	UFUNCTION(BlueprintNativeEvent)
	void OnValueChanged(const FString& NewValue);
	virtual void OnValueChanged_Implementation(const FString& NewValue);

	// Returns a text for a given string in the SelectOptions array
	// This is meant to be overriden (otherwise the string itself will be used)
	UFUNCTION(BlueprintNativeEvent)
	FText GetTextForOption(const FString& OptionStr);
	virtual FText GetTextForOption_Implementation(const FString& OptionStr);
	
	UFUNCTION(BlueprintPure)
	int GetCurrentSelectIndex() const;

	UFUNCTION(BlueprintPure)
	TArray<FString> GetSelectOptions() const { return SelectOptions; }

	// Get options as texts. Override 'GetTextForOption' to customize behavior
	UFUNCTION(BlueprintPure)
	TArray<FText> GetSelectOptionsAsTexts() const { return SelectOptionsTexts; }

protected:
	// Multiple options to select from
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> SelectOptions;

	UPROPERTY()
	TArray<FText> SelectOptionsTexts;
	
	UFUNCTION(BlueprintNativeEvent)
	void InitSelectOptions();
	virtual void InitSelectOptions_Implementation();

	void InitTextOptions();

private:
	UFUNCTION()
	void OnSettingChangeValue_Event(USettingObject* SettingObject);
};
