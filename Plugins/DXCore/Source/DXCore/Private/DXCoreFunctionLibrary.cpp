// Fill out your copyright notice in the Description page of Project Settings.


#include "DXCoreFunctionLibrary.h"

#include "Core/DXCoreGISubsystem.h"
#include "Core/DXCoreWorldSubsystem.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"
#include "Math/BasicMathExpressionEvaluator.h"

FTimerHandle UDXCoreFunctionLibrary::SetIndependentTimerByEvent(FTimerDynamicDelegate Delegate, float Time,
	bool bLooping, float InitialStartDelay, float InitialStartDelayVariance)
{
	FTimerHandle Handle;
	if (Delegate.IsBound())
	{
		const UWorld* const World = GEngine->GetWorldFromContextObject(Delegate.GetUObject(), EGetWorldErrorMode::LogAndReturnNull);
		if(World)
		{
			InitialStartDelay += FMath::RandRange(-InitialStartDelayVariance, InitialStartDelayVariance);
			if (Time <= 0.f || (Time + InitialStartDelay) < 0.f)
			{
				FString ObjectName = GetNameSafe(Delegate.GetUObject());
				FString FunctionName = Delegate.GetFunctionName().ToString(); 
				FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s %s SetTimer passed a negative or zero time. The associated timer may fail to be created/fire! If using InitialStartDelayVariance, be sure it is smaller than (Time + InitialStartDelay)."), *ObjectName, *FunctionName), ELogVerbosity::Warning);
			}
			
			World->GetTimerManager().SetTimer(Handle, Delegate, Time, bLooping, (Time + InitialStartDelay));
		}
	}
	else
	{
		UE_LOG(LogBlueprintUserMessages, Warning, 
			TEXT("SetTimer passed a bad function (%s) or object (%s)"),
			*Delegate.GetFunctionName().ToString(), *GetNameSafe(Delegate.GetUObject()));
	}

	return Handle;
}

bool UDXCoreFunctionLibrary::IsEditor()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}

bool UDXCoreFunctionLibrary::HasActorBegunPlay(const AActor* Actor)
{
	return Actor->HasActorBegunPlay();
}

TArray<FName> UDXCoreFunctionLibrary::GetStreamingLevelNames(const UObject* WorldContextObject)
{
	TArray<FName> streamingLevelNames;
	if(UWorld* world = WorldContextObject->GetWorld())
	{
		auto streamingLevels = world->GetStreamingLevels();
		for(auto& streamingLevel : streamingLevels)
		{
			FName levelName = streamingLevel->PackageNameToLoad.IsNone() ? streamingLevel->GetWorldAssetPackageFName() : streamingLevel->PackageNameToLoad;
			streamingLevelNames.Add(levelName);
		}
	}
	return streamingLevelNames;
}

void UDXCoreFunctionLibrary::TryReplaceWordsForNumbers(FString phrase, const AActor* reference, FString& output,
                                                       FName target)
{
	output = phrase;
	if(!reference)
		return;

	//TArray<FName> names;
	UParameterKeeper* parameterKeeper = Cast<UParameterKeeper>(reference->GetComponentByClass(UParameterKeeper::StaticClass()));
	if(!parameterKeeper)
		return;


	FString targetString = target.ToString();

	TArray<FString> intNames;
	parameterKeeper->GetAllIntNames(intNames);
	for (auto& intName : intNames)
	{
		if (!intName.Equals(targetString, ESearchCase::IgnoreCase))
		{
			if (output.Contains(intName))
			{
				output = output.Replace(*intName, *FString::FromInt(parameterKeeper->GetInt(intName)));
			}
		}
	}

	TArray<FString> floatNames;
	parameterKeeper->GetAllFloatNames(floatNames);
	for (auto& floatName : floatNames)
	{
		if (!floatName.Equals(targetString, ESearchCase::IgnoreCase))
		{
			if (output.Contains(floatName))
			{
				output = output.Replace(*floatName, *FString::SanitizeFloat(parameterKeeper->GetFloat(floatName)));
			}
		}
	}
}

bool UDXCoreFunctionLibrary::TryEvaluateExpression(FString expression, float& result)
{
	FBasicMathExpressionEvaluator ev = FBasicMathExpressionEvaluator();
	TValueOrError<double, FExpressionError> answer = ev.Evaluate(*expression);
	if (answer.IsValid())
	{
		result = answer.GetValue();
		return true;
	}
	return false;
}

FName UDXCoreFunctionLibrary::GetRequirementName(FRequirement Requirement)
{
	return Requirement.RequiredDataName;
}

FRequirement UDXCoreFunctionLibrary::NumericToRequirement(FRequirementNumeric NumericRequirement)
{
	return NumericRequirement.ToRequirement();
}

FRequirement UDXCoreFunctionLibrary::TextToRequirement(FRequirementText TextRequirement)
{
	return TextRequirement.ToRequirement();
}

FRequirement UDXCoreFunctionLibrary::BoolToRequirement(FRequirementBool BoolRequirement)
{
	return BoolRequirement.ToRequirement();
}

FRequirementNumeric UDXCoreFunctionLibrary::RequirementToNumeric(FRequirement Requirement)
{
	return FRequirementNumeric(Requirement);
}

FRequirementText UDXCoreFunctionLibrary::RequirementToText(FRequirement Requirement)
{
	return FRequirementText(Requirement);
}

FRequirementBool UDXCoreFunctionLibrary::RequirementToBool(FRequirement Requirement)
{
	return FRequirementBool(Requirement);
}

float UDXCoreFunctionLibrary::GetAnimMontageSectionLength(UAnimMontage* Montage, int32 SectionIndex)
{
	return Montage->GetSectionLength(SectionIndex);
}

FString UDXCoreFunctionLibrary::GetProjectVersion()
{
	FString ProjectVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		ProjectVersion,
		GGameIni
	);
	return ProjectVersion;
}

ADXWorldSettings* UDXCoreFunctionLibrary::GetWorldSettings(const UObject* WorldContextObject)
{
	if (const UWorld* world = WorldContextObject->GetWorld())
	{
		ADXWorldSettings* ws = Cast<ADXWorldSettings>(world->GetWorldSettings());
		return ws;
	}
	return nullptr;
}

ACosmeticEventManager* UDXCoreFunctionLibrary::GetCosmeticEventManager(const UObject* WorldContextObject)
{
	ACosmeticEventManager* cosmeticEventManager = nullptr;
	if (UWorld* world = WorldContextObject->GetWorld())
	{
		UDXCoreWorldSubsystem* worldSubsystem = nullptr;
		worldSubsystem = Cast<UDXCoreWorldSubsystem>(world->GetSubsystem<UDXCoreWorldSubsystem>());
		cosmeticEventManager = worldSubsystem->GetCosmeticEventManager();
	}
	return cosmeticEventManager;
}

void UDXCoreFunctionLibrary::LocalTriggerInstantCosmeticEvent(TSubclassOf<UInstantCosmeticEvent> eventClass,
	const FCosmeticEventData& parameters, const UObject* WorldContextObject)
{
	if(auto cosmeticEventManager = GetCosmeticEventManager(WorldContextObject))
	{
		cosmeticEventManager->LocalTriggerInstantCosmeticEvent(eventClass, parameters);
	}
}

void UDXCoreFunctionLibrary::AuthTriggerInstantCosmeticEvent(TSubclassOf<UInstantCosmeticEvent> eventClass,
	const FCosmeticEventData& parameters, const TArray<APlayerController*>& clientsToIgnore,
	const UObject* WorldContextObject)
{
	if(auto cosmeticEventManager = GetCosmeticEventManager(WorldContextObject))
	{
		cosmeticEventManager->AuthTriggerInstantCosmeticEvent(eventClass, parameters, clientsToIgnore);
	}
}

ALatentCosmeticEvent* UDXCoreFunctionLibrary::LocalTriggerLatentCosmeticEvent(
	TSubclassOf<ALatentCosmeticEvent> eventClass, const FCosmeticEventData& parameters, FTransform SpawnTransform,
	const UObject* WorldContextObject, int32& OutLatentCosmeticEventID)
{
	int32 GeneratedID;
	if(auto cosmeticEventManager = GetCosmeticEventManager(WorldContextObject))
	{
		auto latentCosmeticEvent = cosmeticEventManager->LocalTriggerLatentCosmeticEvent(eventClass, parameters, SpawnTransform, GeneratedID);
		OutLatentCosmeticEventID = GeneratedID;
		return latentCosmeticEvent;
	}
	return nullptr;
}

ALatentCosmeticEvent* UDXCoreFunctionLibrary::AuthTriggerLatentCosmeticEvent(
	TSubclassOf<ALatentCosmeticEvent> eventClass, const FCosmeticEventData& parameters, FTransform SpawnTransform, const UObject* WorldContextObject, int32& OutLatentCosmeticEventID, int32 InputLatentCosmeticEventID)
{
	if(auto cosmeticEventManager = GetCosmeticEventManager(WorldContextObject))
	{
		return cosmeticEventManager->AuthTriggerLatentCosmeticEvent(eventClass, parameters, SpawnTransform, OutLatentCosmeticEventID, InputLatentCosmeticEventID);
	}
	return nullptr;
}

ALatentCosmeticEvent* UDXCoreFunctionLibrary::GetLatentCosmeticEventFromID(int32 LatentCosmeticEventID,
	const UObject* WorldContextObject)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ALatentCosmeticEvent::StaticClass(), FoundActors);
	for(AActor* actor : FoundActors)
	{
		ALatentCosmeticEvent* latentCosmeticEvent = Cast<ALatentCosmeticEvent>(actor);
		if(latentCosmeticEvent && latentCosmeticEvent->EventID == LatentCosmeticEventID)
		{
			return latentCosmeticEvent;
		}
	}
	return nullptr;
}

bool UDXCoreFunctionLibrary::FindCosmeticEventBool(FCosmeticEventData data, FString key, bool& Value)
{
	return data.FindBool(key, Value);
}

bool UDXCoreFunctionLibrary::FindCosmeticEventInt(FCosmeticEventData data, FString key, int32& Value)
{
	return data.FindInt(key, Value);
}

bool UDXCoreFunctionLibrary::FindCosmeticEventFloat(FCosmeticEventData data, FString key, float& Value)
{
	return data.FindFloat(key, Value);
}

bool UDXCoreFunctionLibrary::FindCosmeticEventString(FCosmeticEventData data, FString key, FString& Value)
{
	return data.FindString(key, Value);
}

bool UDXCoreFunctionLibrary::FindCosmeticEventVector(FCosmeticEventData data, FString key, FVector& Value)
{
	return data.FindVector(key, Value);
}

bool UDXCoreFunctionLibrary::FindCosmeticEventRotator(FCosmeticEventData data, FString key, FRotator& Value)
{
	return data.FindRotator(key, Value);
}

bool UDXCoreFunctionLibrary::FindCosmeticEventTransform(FCosmeticEventData data, FString key, FTransform& Value)
{
	return data.FindTransform(key, Value);
}

bool UDXCoreFunctionLibrary::FindCosmeticEventObject(FCosmeticEventData data, FString key, UObject*& Value)
{
	return data.FindObject(key, Value);
}
