// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "DXCoreImportantTypes.h"
#include "CosmeticEventSystem/CosmeticEventManager.h"
#include "Core/DXCoreSettings.h"
#include "Core/DXWorldSettings.h"
#include "DXCoreFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DXCORE_API UDXCoreFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/* https://forums.unrealengine.com/t/how-do-i-run-the-same-event-multiple-times-independently-of-each-other/537075/5
	 *  This will create a new handle everytime it's called. Original K2_SetTimerByEvent will search for existing handle and will cancel previously set timer.
	 */
	UFUNCTION(BlueprintCallable, meta=(DisplayName = "Set Independent Timer by Event", ScriptName = "SetIndependentTimerDelegate", AdvancedDisplay="InitialStartDelay, InitialStartDelayVariance"), Category="Utilities|Time")
	static FTimerHandle SetIndependentTimerByEvent(UPARAM(DisplayName="Event") FTimerDynamicDelegate Delegate, float Time, bool bLooping, float InitialStartDelay = 0.f, float InitialStartDelayVariance = 0.f);
	
	UFUNCTION(BlueprintPure, Category="Utilities")
	static bool IsEditor();

	UFUNCTION(BlueprintPure, Category="Utilities")
	static bool HasActorBegunPlay(const AActor* Actor);
	
	UFUNCTION(BlueprintPure, Category = "World", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static TArray<FName> GetStreamingLevelNames(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "DXCore|RequirementSystem")
	static void TryReplaceWordsForNumbers(FString phrase, const AActor* reference, FString &output, FName target = FName(""));
	
	UFUNCTION(BlueprintPure, Category = "DXCore|RequirementSystem")
	static bool TryEvaluateExpression(FString expression, float &result);

	UFUNCTION(BlueprintPure, Category = "DXCore|RequirementSystem")
	static FName GetRequirementName(FRequirement Requirement);
	
	UFUNCTION(BlueprintPure, Category = "DXCore|RequirementSystem")
	static FRequirement NumericToRequirement(FRequirementNumeric NumericRequirement);
	
	UFUNCTION(BlueprintPure, Category = "DXCore|RequirementSystem")
	static FRequirement TextToRequirement(FRequirementText TextRequirement);
	
	UFUNCTION(BlueprintPure, Category = "DXCore|RequirementSystem")
	static FRequirement BoolToRequirement(FRequirementBool BoolRequirement);
	
	UFUNCTION(BlueprintPure, Category = "DXCore|RequirementSystem")
	static FRequirementNumeric RequirementToNumeric(FRequirement Requirement);

	UFUNCTION(BlueprintPure, Category = "DXCore|RequirementSystem")
	static FRequirementText RequirementToText(FRequirement Requirement);
	
	UFUNCTION(BlueprintPure, Category = "DXCore|RequirementSystem")
	static FRequirementBool RequirementToBool(FRequirement Requirement);

	UFUNCTION(BlueprintCallable, Category = "DXCore|AnimMontage")
	static float GetAnimMontageSectionLength(UAnimMontage* Montage, int32 SectionIndex);

	UFUNCTION(BlueprintPure, Category = "Information")
	static FString GetProjectVersion();

	UFUNCTION(BlueprintPure, Category = "World", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static ADXWorldSettings* GetWorldSettings(const UObject* WorldContextObject);

	//Wrapper functions for all Finder functions in FCosmeticEventData

	//==== Cosmetic Event System
	
private:

	static ACosmeticEventManager* GetCosmeticEventManager(const UObject* WorldContextObject);

public:

	UFUNCTION(BlueprintCallable, Category = "DXCore|Cosmetic Event System", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ToolTip="Prefer CosmeticEventsComponent."))
	static void LocalTriggerInstantCosmeticEvent(TSubclassOf<UInstantCosmeticEvent> eventClass, const FCosmeticEventData& parameters, const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DXCore|Cosmetic Event System", meta = (AutoCreateRefTerm = "clientsToIgnore", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ToolTip="Prefer CosmeticEventsComponent."))
	static void AuthTriggerInstantCosmeticEvent(TSubclassOf<UInstantCosmeticEvent> eventClass, const FCosmeticEventData& parameters, const TArray<APlayerController*>& clientsToIgnore, const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "DXCore|Cosmetic Event System", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ToolTip="Prefer CosmeticEventsComponent."))
	static ALatentCosmeticEvent* LocalTriggerLatentCosmeticEvent(TSubclassOf<ALatentCosmeticEvent> eventClass, const FCosmeticEventData& parameters, FTransform SpawnTransform, const UObject* WorldContextObject, int32& OutLatentCosmeticEventID);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DXCore|Cosmetic Event System", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ToolTip="Prefer CosmeticEventsComponent."))
	static ALatentCosmeticEvent* AuthTriggerLatentCosmeticEvent(TSubclassOf<ALatentCosmeticEvent> eventClass, const FCosmeticEventData& parameters, FTransform SpawnTransform, const UObject* WorldContextObject, int32& OutLatentCosmeticEventID, int32 InputLatentCosmeticEventID = -1);
	
	UFUNCTION(BlueprintPure, Category = "DXCore|Cosmetic Event System", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", ToolTip = "Beware! The reference is ephemeral and should not be cached / stored, e.g. local versions could be destroyed for server reconciliation when replicated version arrives. Instead always use the EventID in this getter to get the event reference.", ShortTip = "Get the event reference from the EventIDm, ephemeral reference."))
	static ALatentCosmeticEvent* GetLatentCosmeticEventFromID(int32 LatentCosmeticEventID, const UObject* WorldContextObject);
	
	//====
	
	UFUNCTION(BlueprintPure, Category = "DXCore|CosmeticEventSystem")
	static bool FindCosmeticEventBool(FCosmeticEventData data, FString key, bool& Value);

	UFUNCTION(BlueprintPure, Category = "DXCore|CosmeticEventSystem")
	static bool FindCosmeticEventInt(FCosmeticEventData data, FString key, int32& Value);

	UFUNCTION(BlueprintPure, Category = "DXCore|CosmeticEventSystem")
	static bool FindCosmeticEventFloat(FCosmeticEventData data, FString key, float& Value);

	UFUNCTION(BlueprintPure, Category = "DXCore|CosmeticEventSystem")
	static bool FindCosmeticEventString(FCosmeticEventData data, FString key, FString& Value);

	UFUNCTION(BlueprintPure, Category = "DXCore|CosmeticEventSystem")
	static bool FindCosmeticEventVector(FCosmeticEventData data, FString key, FVector& Value);

	UFUNCTION(BlueprintPure, Category = "DXCore|CosmeticEventSystem")
	static bool FindCosmeticEventRotator(FCosmeticEventData data, FString key, FRotator& Value);

	UFUNCTION(BlueprintPure, Category = "DXCore|CosmeticEventSystem")
	static bool FindCosmeticEventTransform(FCosmeticEventData data, FString key, FTransform& Value);

	UFUNCTION(BlueprintPure, Category = "DXCore|CosmeticEventSystem")
	static bool FindCosmeticEventObject(FCosmeticEventData data, FString key, UObject*& Value);

	template<typename T>
	static  TMap<FName, T> GetStructMap(UDataTable* dataTable);

	template<typename TEnum>
	static  FString EnumToString(TEnum Value);
};

template <typename TEnum>
FString UDXCoreFunctionLibrary::EnumToString(TEnum Value)
{
	static_assert( TIsUEnumClass< TEnum >::Value, "'TEnum' template parameter to EnumToString must be a valid UEnum" );
	return StaticEnum< TEnum >()->GetNameStringByIndex( ( int32 ) Value );
}


/**
 * @brief Creates a TMap<FName, T> based on an arbitrary Data Table of row type T 
 */
template<typename T>
TMap<FName, T> UDXCoreFunctionLibrary::GetStructMap(UDataTable* dataTable)
{
	TMap<FName, T> structs;
	FDataTableRowHandle tableHandle;
	
	tableHandle.DataTable = dataTable;
	if (tableHandle.DataTable)
	{
		FString ContextString;
		TArray<FName> RowNames;
		RowNames = tableHandle.DataTable->GetRowNames();
		for (auto& name : RowNames)
		{
			T* structPointer = tableHandle.DataTable->FindRow<T>(name, ContextString);
			if (structPointer)
			{
				FName structName = name;
				T structCopy = *structPointer;
				structs.Add(structName, structCopy);
			}
		}
	}
	return structs;
}