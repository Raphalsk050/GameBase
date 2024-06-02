// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DXTweakingDataObject.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class DXTWEAKINGMANAGER_API UDXTweakingSelectNode : public UObject
{
	GENERATED_BODY()

	UDXTweakingSelectNode();

public:
#pragma region Get All Values
	UPROPERTY(BlueprintReadOnly)
	FString RawValue = TEXT("");

	UFUNCTION(BlueprintPure)
	float GetValueAsFloat();

	UFUNCTION(BlueprintPure)
	bool GetValueAsBool();

	UFUNCTION(BlueprintPure)
	int32 GetValueAsInteger();

	UFUNCTION(BlueprintPure)
	FString GetValueAsString();

	UFUNCTION(BlueprintPure)
	FName GetValueAsName();
#pragma endregion
};

UCLASS(Blueprintable)
class DXTWEAKINGMANAGER_API UDXTweakingDataObject : public UObject
{
	GENERATED_BODY()
	UDXTweakingDataObject();
	
public:

	//Original name of the data sheet, for logging purposes
	UPROPERTY(BlueprintReadOnly)
	FString FileOrigin = TEXT("");

	//Array containing the data sheet header
	UPROPERTY(BlueprintReadOnly)
	TArray<FString> DataHeader;

	//Array containing the data nodes, stored continuously in a single array
	UPROPERTY(BlueprintReadOnly)
	TArray<UDXTweakingSelectNode*> DataNodes;

	// Get the number of rows the data object contains
	UFUNCTION(BlueprintPure)
	int32 GetNumberOfRows();

	// Get data nodes from specific row number
	UFUNCTION(BlueprintPure)
	bool GetRow(int32 RowNumber, TArray<UDXTweakingSelectNode*>& OutNodes);

	// Parses the csv from the filepath into this object
	bool ParseCsv(const FString& Filepath);

	private:
	// Adds the specified string into a DataNode, clearing the string in process
	void AddToNode(FString* StringToAdd);

	// Adds the specified string into the header, clearing the string in process
	void AddToHeader(FString* StringToAdd);
	
};
