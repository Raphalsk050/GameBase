// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorUtilitiesLibrary.h"

#include "HAL/PlatformApplicationMisc.h"
#include "Kismet2/ComponentEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Engine/LevelStreaming.h"
#include "Engine/StaticMeshActor.h"
#include "Components/ModelComponent.h"

void UEditorUtilitiesLibrary::CopyStringToClipBoard(FString string)
{
	FPlatformApplicationMisc::ClipboardCopy(*string);
}

TArray<FName> UEditorUtilitiesLibrary::GetLoadedStreamingLevels(UObject* WorldContextObject)
{
	UWorld* world = WorldContextObject->GetWorld();
	TArray<FName> levelNames;
	TArray<ULevelStreaming*> levelStreamings = world->GetStreamingLevels();
	for (ULevelStreaming* ls : levelStreamings)
	{
		if (ls->IsLevelLoaded())
		{
			levelNames.Add(ls->GetWorldAssetPackageFName());
		}
	}
	return levelNames;
}

TArray<AActor*> UEditorUtilitiesLibrary::GetActorsFromLevel(ULevelStreaming* Level)
{
	if (Level->IsLevelLoaded())
	{
		return Level->GetLoadedLevel()->Actors;
	}
	return TArray<AActor*>();
}

void UEditorUtilitiesLibrary::SetupBlockingsForBuildLight(UObject* WorldContextObject, FName VisualBlockingLayerName)
{
	UWorld* world = WorldContextObject->GetWorld();
	TArray<FName> levelNames;
	TArray<ULevelStreaming*> levelStreamings = world->GetStreamingLevels();
	for (ULevelStreaming* ls : levelStreamings)
	{
		if (ls->IsLevelLoaded())
		{
			levelNames.Add(ls->GetWorldAssetPackageFName());
		}
	}
	
	for (FName levelName : levelNames)
	{
		ULevelStreaming* levelStreaming = world->GetLevelStreamingForPackageName(levelName);
		ULevel* level = levelStreaming->GetLoadedLevel();
		if (level)
		{
			if (levelName.ToString().Contains("_Game", ESearchCase::IgnoreCase, ESearchDir::FromEnd))
			{
				for (AActor* actor : level->Actors)
				{
					if (actor->IsValidLowLevel())
					{
						if (!actor->IsChildActor())
						{
							if ((actor->IsA(AStaticMeshActor::StaticClass()) || actor->ActorHasTag(FName("Blocking"))) && !actor->Layers.Contains(VisualBlockingLayerName))
							{
								AStaticMeshActor* smActor = Cast<AStaticMeshActor>(actor);
								if (smActor)
								{
									smActor->GetStaticMeshComponent()->SetVisibility(false, true);
									smActor->GetStaticMeshComponent()->SetCastShadow(false);
									smActor->GetStaticMeshComponent()->LightmapType = ELightmapType::ForceVolumetric;
									actor->MarkPackageDirty();
								}
								TArray<UActorComponent*> smComponents;
								actor->GetComponents(UStaticMeshComponent::StaticClass(), smComponents);
								for (auto& smComp : smComponents) {
									UStaticMeshComponent* smCompCast = Cast<UStaticMeshComponent>(smComp);
									if (smCompCast) {
										smCompCast->SetVisibility(false, true);
										smCompCast->SetCastShadow(false);
										smCompCast->LightmapType = ELightmapType::ForceVolumetric;
										actor->MarkPackageDirty();
									}
								}
							}
						}
					}
				}
				for (UModelComponent* mc : level->ModelComponents)
				{
					if (mc->GetOwner() && !mc->GetOwner()->Layers.Contains(VisualBlockingLayerName))
					{
						mc->SetVisibility(false, true);
						mc->SetCastShadow(false);
						mc->LightmapType = ELightmapType::ForceVolumetric;
						mc->MarkPackageDirty();
					}
				}
			}
		}
	}
}

void UEditorUtilitiesLibrary::UnSetupBlockingsForBuildLight(UObject* WorldContextObject, FName VisualBlockingLayerName)
{
	UWorld* world = WorldContextObject->GetWorld();
	TArray<FName> levelNames;
	TArray<ULevelStreaming*> levelStreamings = world->GetStreamingLevels();
	for (ULevelStreaming* ls : levelStreamings)
	{
		if (ls->IsLevelLoaded())
		{
			levelNames.Add(ls->GetWorldAssetPackageFName());
		}
	}

	for (FName levelName : levelNames)
	{
		ULevelStreaming* levelStreaming = world->GetLevelStreamingForPackageName(levelName);
		ULevel* level = levelStreaming->GetLoadedLevel();
		if (level)
		{
			if (levelName.ToString().Contains("_Game", ESearchCase::IgnoreCase, ESearchDir::FromEnd))
			{
				for (AActor* actor : level->Actors)
				{
					if (actor->IsValidLowLevel())
					{
						if (!actor->IsChildActor())
						{
							if ((actor->IsA(AStaticMeshActor::StaticClass()) || actor->ActorHasTag(FName("Blocking"))) && !actor->Layers.Contains(VisualBlockingLayerName))
							{
								AStaticMeshActor* smActor = Cast<AStaticMeshActor>(actor);
								if (smActor)
								{
									smActor->GetStaticMeshComponent()->LightmapType = ELightmapType::Default;
									smActor->GetStaticMeshComponent()->SetCastShadow(true);
									if (!smActor->GetStaticMeshComponent()->bCastHiddenShadow)
									{
										smActor->GetStaticMeshComponent()->SetVisibility(true, true);
									}
									smActor->MarkPackageDirty();
								}

								TArray<UActorComponent*> smComponents;
								actor->GetComponents<>(UStaticMeshComponent::StaticClass(), smComponents);
								for (auto& smComp : smComponents) 
								{
									UStaticMeshComponent* smCompCast = Cast<UStaticMeshComponent>(smComp);
									if (smCompCast) 
									{
										smCompCast->LightmapType = ELightmapType::Default;
										smCompCast->SetCastShadow(true);
										if (!smCompCast->bCastHiddenShadow)
										{
											smCompCast->SetVisibility(true, true);
										}
										actor->MarkPackageDirty();
									}
								}
							}
						}
					}
				}
				for (UModelComponent* mc : level->ModelComponents)
				{
					if (mc->GetOwner() && !mc->GetOwner()->Layers.Contains(VisualBlockingLayerName))
					{
						mc->SetCastShadow(true);
						mc->LightmapType = ELightmapType::Default;

						if (!mc->bCastHiddenShadow)
						{
							mc->SetVisibility(true, true);
						}
						mc->MarkPackageDirty();
					}
				}
			}
		}
	}
}

UActorComponent* UEditorUtilitiesLibrary::AddInstanceActorComponent(AActor* actorInstance,
	TSubclassOf<UActorComponent> componentClass)
{
	if (!actorInstance) {
		return nullptr;
	}
	if (!componentClass) {
		return nullptr;
	}
	actorInstance->Modify();

	// Get the set of owned components that exists prior to instancing the new component.
	TInlineComponentArray<UActorComponent*> PreInstanceComponents;
	actorInstance->GetComponents(PreInstanceComponents);
	
	FName NewComponentName = NAME_None;
	NewComponentName = *FComponentEditorUtils::GenerateValidVariableName(componentClass, actorInstance);
	UActorComponent* NewInstanceComponent = NewObject<UActorComponent>(actorInstance, componentClass, NewComponentName);

	actorInstance->AddInstanceComponent(NewInstanceComponent);
	NewInstanceComponent->OnComponentCreated();
	NewInstanceComponent->RegisterComponent();

	// Register any new components that may have been created during construction of the instanced component, but were not explicitly registered.
	TInlineComponentArray<UActorComponent*> PostInstanceComponents;
	actorInstance->GetComponents(PostInstanceComponents);
	for (UActorComponent* ActorComponent : PostInstanceComponents)
	{
		if (!ActorComponent->IsRegistered() && ActorComponent->bAutoRegister && IsValid(ActorComponent) && !PreInstanceComponents.Contains(ActorComponent))
		{
			ActorComponent->RegisterComponent();
		}
	}

	// Rerun construction scripts
	actorInstance->RerunConstructionScripts();

	return NewInstanceComponent;
}

bool UEditorUtilitiesLibrary::IsActorComponentNativeCreated(UActorComponent* ActorComponent)
{
	return ActorComponent->CreationMethod == EComponentCreationMethod::Native;
}

bool UEditorUtilitiesLibrary::IsActorComponentBlueprintCreated(UActorComponent* ActorComponent)
{
	return ActorComponent->CreationMethod == EComponentCreationMethod::SimpleConstructionScript;
}

bool UEditorUtilitiesLibrary::IsActorComponentConstructionScriptCreated(UActorComponent* ActorComponent)
{
	return ActorComponent->CreationMethod == EComponentCreationMethod::UserConstructionScript;
}

bool UEditorUtilitiesLibrary::IsActorComponentInstanceCreated(UActorComponent* ActorComponent)
{
	return ActorComponent->CreationMethod == EComponentCreationMethod::Instance;
}
