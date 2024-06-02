// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DXTweakingManagerSubsystem.generated.h"

const FString SpreadsheetBaseURL = "https://docs.google.com/spreadsheets/d/";

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGoogleDB, FString, DataBase);

UENUM()
enum class EPossibleSheetNames : uint8
{
	Abilities,
	Characters,
	GameModes,
	ALL
};
ENUM_RANGE_BY_COUNT(EPossibleSheetNames, EPossibleSheetNames::ALL);

UCLASS()
class DXTWEAKINGMANAGER_API UDXTweakingManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void ConfigNotionRequisition(FString Key, FString DataBase, FString NotionVersion, FString type);

	UFUNCTION(BlueprintCallable)
	void UpdateProperties(FString Name, EPossibleSheetNames Sheet = EPossibleSheetNames::ALL);
	
	UFUNCTION(BlueprintCallable)
	void RequestNotionsConnection();

	UFUNCTION(BlueprintPure)
	FString GoogleSheetDataBase();

	UPROPERTY(BlueprintAssignable)
	FGoogleDB OnGoogleDB;

	void GetSheetAsCSV(EPossibleSheetNames ChosenSheet);
	
protected:
	void OnNotionsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnected);

	void OnSheetCSVReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void UpdateFor();
	
private:

	FString NotionUrl;
	FString BearerValue;
	FString NotionKey;
	FString NotionDataBase;
	FString Version;
	FString NotionObjectType;
	FString ID;
	FString GoogleDataBase = TEXT("");
	TArray<FString> NotionMatriz;
	TArray<FString> SheetMatriz;
	TArray<FString> EndNotionMatriz;
	bool protectCall;
	
	/* ID of the google spreadsheet we want */
	FString SpreadsheetID;

	/* Name of the sheet inside the spreadsheet we want (can be empty) */
	EPossibleSheetNames SheetName;

	FString CSVContentAsString;
};
