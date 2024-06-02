// Fill out your copyright notice in the Description page of Project Settings.


#include "DXTweakingManagerSubsystem.h"

#include "DXCoreFunctionLibrary.h"
#include "DXTweakingManagerComponent.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UDXTweakingManagerSubsystem::ConfigNotionRequisition(FString Key, FString DataBase, FString NotionVersion, FString type)
{
	NotionKey = Key;
	NotionDataBase = DataBase;
	Version = NotionVersion;
	NotionObjectType = type;
	//Set Url
	const FString InitUrl = TEXT("https://api.notion.com/v1/databases/");
	NotionUrl = InitUrl + NotionDataBase + TEXT("/query");
	//Set Authorization
	BearerValue = TEXT("Bearer ") + NotionKey;

	SheetName = EPossibleSheetNames::ALL;
}

void UDXTweakingManagerSubsystem::UpdateProperties(FString Name, const EPossibleSheetNames Sheet)
{
	ID = Name;
	SheetName = Sheet;
	RequestNotionsConnection();
}

void UDXTweakingManagerSubsystem::RequestNotionsConnection()
{
	//disallow multiple requests
	if(protectCall) return;

	protectCall = true;
	
	const FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("POST");
	Request->SetURL(NotionUrl);
	Request->SetHeader("Authorization", BearerValue);
	Request->SetHeader("Notion-Version", Version);
	Request->SetHeader("content-type", NotionObjectType);
	Request->ProcessRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UDXTweakingManagerSubsystem::OnNotionsResponseReceived);
}

void UDXTweakingManagerSubsystem::OnNotionsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response,
															bool bConnected)
{
	if (bConnected == false) return;

	protectCall = false;
	
	Response->GetContentAsString().ParseIntoArray(NotionMatriz, TEXT(","), true);
	Response->GetContentAsString().ParseIntoArray(NotionMatriz, TEXT(":"), true);
	Response->GetContentAsString().ParseIntoArray(NotionMatriz, TEXT(R"(")"), true);
	
	for(FString String : NotionMatriz)
	{
		String.ParseIntoArray(EndNotionMatriz, TEXT("@"), true);
		//SecondNotionMatriz.Add(String);
		for (FString String2 : EndNotionMatriz)
		{
			int32 index;
			EndNotionMatriz.Find(ID, index);
			if (EndNotionMatriz.IsValidIndex(index))
			{
				GoogleDataBase = EndNotionMatriz[index +1];
				OnGoogleDB.Broadcast(GoogleDataBase);
				//GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, GoogleDataBase);
			}
		}
	}
	
	GetSheetAsCSV(SheetName);
}


void UDXTweakingManagerSubsystem::GetSheetAsCSV(EPossibleSheetNames ChosenSheet)
{
	FString BaseURL = SpreadsheetBaseURL + GoogleDataBase + "/gviz/tq?tqx=out:csv";
	FString SelectedSheet = TEXT("");

	//we want all the sheets, so perform a request for each
	if(ChosenSheet == EPossibleSheetNames::ALL)
	{
		UE_LOG(LogTemp, Display, TEXT("%s: Getting all sheets.."), ANSI_TO_TCHAR(__FUNCTION__));
		
		for (const EPossibleSheetNames Sheet : TEnumRange<EPossibleSheetNames>())
		{
			if(Sheet != EPossibleSheetNames::ALL)
			{
				//we need to make a separated request for each
				const FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
				Request->SetVerb("GET");
				Request->OnProcessRequestComplete().BindUObject(this, &UDXTweakingManagerSubsystem::OnSheetCSVReceived);
				
				SelectedSheet = UDXCoreFunctionLibrary::EnumToString(Sheet);

				FString UpdatedURL = BaseURL;
				UpdatedURL += TEXT("&sheet=") + SelectedSheet;
				Request->SetURL(UpdatedURL);
				Request->ProcessRequest();
			}
		}
	}
	//we want a specific sheet, perform one request for the selected
	else
	{
		UE_LOG(LogTemp, Display, TEXT("%s: Getting sheet %s"), ANSI_TO_TCHAR(__FUNCTION__), *SelectedSheet);
		
		const FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
		Request->SetVerb("GET");
		Request->OnProcessRequestComplete().BindUObject(this, &UDXTweakingManagerSubsystem::OnSheetCSVReceived);
		
		SelectedSheet = UDXCoreFunctionLibrary::EnumToString(ChosenSheet);
		BaseURL += TEXT("&sheet=") + SelectedSheet;
		Request->SetURL(BaseURL);
		Request->ProcessRequest();
	}
}

void UDXTweakingManagerSubsystem::OnSheetCSVReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		UE_LOG(LogTemp, Display, TEXT("%s: success receiving sheet"), ANSI_TO_TCHAR(__FUNCTION__));
		
		TArray<FString> GoogleParse;
		Response->GetContentAsString().ParseIntoArray(SheetMatriz, TEXT(R"(")"), true);
		for(FString String : SheetMatriz)
		{
			GoogleParse.Add(String);
			CSVContentAsString += String;
		}
		UpdateFor();
	}
}

void UDXTweakingManagerSubsystem::UpdateFor()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(),FoundActors);
	for (AActor* TActor: FoundActors)
	{
		auto* Target = Cast<AActor>(TActor);
		if(Target == nullptr)
			return;
		if(auto Component = Target->FindComponentByClass<UDXTweakingManagerComponent>())
		{
			Component->PlayTweakingManagerAsCSV(CSVContentAsString);
		}
	}
}

FString UDXTweakingManagerSubsystem::GoogleSheetDataBase()
{
	return GoogleDataBase;
}



