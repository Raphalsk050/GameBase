// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EditorUtilitiesLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DXCOREEDITOR_API UEditorUtilitiesLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Editor Utilities Library")
	static void CopyStringToClipBoard(FString string);

	UFUNCTION(BlueprintCallable, Category = "Editor Utilities Library", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static TArray<FName> GetLoadedStreamingLevels(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Editor Utilities Library")
	static TArray<AActor*> GetActorsFromLevel(ULevelStreaming* Level);

	UFUNCTION(BlueprintCallable, Category = "Editor Utilities Library", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void SetupBlockingsForBuildLight(UObject * WorldContextObject, FName VisualBlockingLayerName);

	UFUNCTION(BlueprintCallable, Category = "Editor Utilities Library", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static void UnSetupBlockingsForBuildLight(UObject * WorldContextObject, FName VisualBlockingLayerName);

	UFUNCTION(BlueprintCallable, Category="Editor Utilities Library")
	static UActorComponent* AddInstanceActorComponent(AActor* actorInstance, TSubclassOf<UActorComponent> componentClass);

	UFUNCTION(BlueprintPure, Category="Editor Utilities Library")
	static bool IsActorComponentNativeCreated(UActorComponent* ActorComponent);

	UFUNCTION(BlueprintPure, Category="Editor Utilities Library")
	static bool IsActorComponentBlueprintCreated(UActorComponent* ActorComponent);

	UFUNCTION(BlueprintPure, Category="Editor Utilities Library")
	static bool IsActorComponentConstructionScriptCreated(UActorComponent* ActorComponent);
	
	UFUNCTION(BlueprintPure, Category="Editor Utilities Library")
	static bool IsActorComponentInstanceCreated(UActorComponent* ActorComponent);
};
