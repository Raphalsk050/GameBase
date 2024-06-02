// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/DXCoreGISubsystem.h"

#include <sstream>

#include "DXCoreImportantTypes.h"
#include "Components/ModelComponent.h"
#include "Core/DXWorldSettings.h"
#include "Engine/LevelStreaming.h"
#include "Engine/StaticMeshActor.h"
#include "OnlineSubsystem.h"
#include "Core/DXCoreSettings.h"
#include "Core/DXCoreWorldSubsystem.h"
#include "CosmeticEventSystem/InstantCosmeticEvent.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineExternalUIInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "RequirementSystem/Requirable.h"

void UDXCoreGISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	FWorldDelegates::LevelAddedToWorld.AddUObject(this, &UDXCoreGISubsystem::LevelAdded);
	FWorldDelegates::LevelRemovedFromWorld.AddUObject(this, &UDXCoreGISubsystem::LevelRemoved);

	const UDXCoreSettings* settings = GetDefault<UDXCoreSettings>();
	if(settings->SaveGameTargetVersion.IsEmpty() || !settings->SaveGameVersions.Contains(settings->SaveGameTargetVersion))
	{
		bSaveSystemIsCorrupt = true;
	}

	SaveGameTargetVersion = settings->SaveGameTargetVersion;
	
	if(!bSaveSystemIsCorrupt)
	{
		MainSaveGameClass = settings->SaveGameVersions[SaveGameTargetVersion].MainSaveGameClass;
	}
	
	OnUserReset.AddDynamic(this, &UDXCoreGISubsystem::OnReceiveUserReset);

	FTickerDelegate TickerDelegate = FTickerDelegate::CreateUObject(this, &UDXCoreGISubsystem::TickDelegate);
	TickerDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(TickerDelegate);
}

void UDXCoreGISubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UDXCoreGISubsystem::TickDelegate(float DeltaTime)
{
	_gameTime += DeltaTime;
	return true;
}

void UDXCoreGISubsystem::LevelAdded(ULevel* level, UWorld* world)
{
	currentWorld = world;

	UpdateLevelActors(level, world, level->GetOuter()->GetName());

	if(const auto levelStreaming = ULevelStreaming::FindStreamingLevel(level))
	{
		OnLevelLoadChanged.Broadcast(levelStreaming->GetWorldAssetPackageFName(), true);
	}
}

void UDXCoreGISubsystem::LevelRemoved(ULevel* level, UWorld* world)
{
	if(const auto levelStreaming = ULevelStreaming::FindStreamingLevel(level))
	{
		OnLevelLoadChanged.Broadcast(levelStreaming->GetWorldAssetPackageFName(), false);
	}
}

const FName UDXCoreGISubsystem::VisualBlockingLayerName = FName(TEXT("VisualBlocking"));

void UDXCoreGISubsystem::UpdateLevelActors(ULevel* level, UWorld* world, FString levelName) const
{
	if(!world)
		return;
	
	ADXWorldSettings* worldSettings = Cast<ADXWorldSettings>(world->GetWorldSettings());
	if(!worldSettings)
		return;
	const ELevelPlayState playState = worldSettings->GetLevelPlayState();

	if (!level)
		return;	

	if (playState == ELevelPlayState::LPS_Final)
	{
		if (levelName.Contains("_Env"))
		{
			for (AActor* actor : level->Actors)
			{
				if (!actor->IsChildActor())
					actor->SetActorEnableCollision(false);
			}
			for (UModelComponent* mc : level->ModelComponents)
			{
				mc->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}

		}
		else if (levelName.Contains("_Game"))
		{
			for (AActor* actor : level->Actors)
			{
				/*

				* CRASH'S INFO: 
				*
				* Here goes a Luke's quick tip: 
				* When crashes occur in this section, 
				* there's a chance of null actors existence in map files 
				* caused by removing assets without dependencies protection; 

				* But don't worry, padawan.
				* try re-saving all the maps,
				* then, re-open and play again,
				* so your trouble will be solved perhaps.

				*/
				if(actor && actor->IsValidLowLevelFast())
				{
					if (!actor->IsChildActor())
					{
						if ((actor->IsA(AStaticMeshActor::StaticClass()) || actor->ActorHasTag(FName("Blocking"))) && !actor->Layers.Contains(VisualBlockingLayerName))
						{
							actor->SetActorHiddenInGame(true);
						}
					}
				}
			}
			for (UModelComponent* mc : level->ModelComponents)
			{
				if (mc->GetOwner() && !mc->GetOwner()->Layers.Contains(VisualBlockingLayerName))
				{
					mc->SetHiddenInGame(true);
				}
			}
		}

	}
	else if (playState == ELevelPlayState::LPS_Blocking)
	{
		if (levelName.Contains("_Env"))
		{
			for (AActor* actor : level->Actors)
			{
				if (!actor->IsChildActor())
				{
					actor->SetActorEnableCollision(false);
					actor->SetActorHiddenInGame(true);
				}
			}
			for (UModelComponent* mc : level->ModelComponents)
			{
				mc->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}

void UDXCoreGISubsystem::OnReceiveUserReset()
{
	/*
	dataKeeper = FGlobalDataKeeper();
	_activeAdventure = FName();
	_lastActiveAdventure = FName();
	_adventuresCurrentLocations.Empty();
	_gameSave = nullptr;*/
}

void UDXCoreGISubsystem::SetActiveLocalPlayer(ULocalPlayer* newActiveLocalPlayer)
{
	_lastActiveLocalPlayer = _activeLocalPlayer;
	_activeLocalPlayer = newActiveLocalPlayer;
}

ULocalPlayer* UDXCoreGISubsystem::GetActiveLocalPlayer()
{
	return _activeLocalPlayer;
}

ULocalPlayer* UDXCoreGISubsystem::GetLastActiveLocalPlayer()
{
	return _lastActiveLocalPlayer;
}

APlayerController* UDXCoreGISubsystem::GetActivePlayerController() const
{
	UWorld* world = GetWorld();
	if (world && _activeLocalPlayer)
	{
		APlayerController* PC = _activeLocalPlayer->GetPlayerController(world);
		if (PC)
		{
			return PC;
		}
	}
	return nullptr;
}

int32 UDXCoreGISubsystem::GetPlayerPlatformID(ULocalPlayer* localPlayer)
{
	if (localPlayer && localPlayer->GetControllerId() != -1) {
		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::GetByPlatform();
		if (OnlineSub)
		{
			IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
			if (Identity.IsValid())
			{
				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(localPlayer->GetControllerId());
				if (UserId.IsValid())
				{
					FPlatformUserId platId = Identity->GetPlatformUserIdFromUniqueNetId(*UserId.Get());
					if (platId == PLATFORMUSERID_NONE)
						return -1;
					return platId;
				}
				else
				{
					UE_LOG(LogOnline, Warning, TEXT("GetPlayerPlatformID - No valid user id for this controller. Controller Id is: %d."), localPlayer->GetControllerId());
				}
			}
			else
			{
				UE_LOG(LogOnline, Warning, TEXT("GetPlayerPlatformID - No valid identity interface."));
			}
		}
		else
		{
			UE_LOG(LogOnline, Warning, TEXT("GetPlayerPlatformID - No default online subsystem."));
			return 0;
		}
	}
	else
	{
		UE_LOG(LogOnline, Warning, TEXT("GetPlayerPlatformID - No local player, cannot read achievements."));
	}

	return -1;
}

TSharedPtr<const FUniqueNetId> UDXCoreGISubsystem::GetPlayerUniqueNetId(ULocalPlayer* localPlayer)
{
	if (localPlayer && localPlayer->GetControllerId() != -1) {
		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
		if (OnlineSub)
		{
			IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
			if (Identity.IsValid())
			{
				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId(localPlayer->GetControllerId());
				if (UserId.IsValid())
				{
					return UserId;
				}
				else
				{
					UE_LOG(LogOnline, Warning, TEXT("GetPlayerUniqueNetId - No valid user id for this controller. Controller Id is: %d."), localPlayer->GetControllerId());
				}
			}
			else
			{
				UE_LOG(LogOnline, Warning, TEXT("GetPlayerUniqueNetId - No valid identity interface."));
			}
		}
		else
		{
			UE_LOG(LogOnline, Warning, TEXT("GetPlayerUniqueNetId - No default online subsystem."));
		}
	}
	else
	{
		UE_LOG(LogOnline, Warning, TEXT("GetPlayerUniqueNetId - local player invalid."));
	}

	return nullptr;
}

void UDXCoreGISubsystem::ShowLoginUIForActiveController()
{
	if (_activeLocalPlayer && _activeLocalPlayer->GetControllerId() != -1) {
		IOnlineSubsystem* OnlineSub = IOnlineSubsystem::GetByPlatform();
		if (OnlineSub)
		{
			IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
			if (Identity.IsValid())
			{
				const auto ExternalUI = OnlineSub->GetExternalUIInterface();
				if (ExternalUI.IsValid())
				{
					//ExternalUI->ShowLoginUI
					//MyUselessInGameMenu = MakeShareable(new FMyUselessSlate());
					ExternalUI->ShowLoginUI(_activeLocalPlayer->GetControllerId(), false, true);
				}
			}
			else
			{
				UE_LOG(LogOnline, Warning, TEXT("No valid identity interface."));
			}
		}
		else
		{
			UE_LOG(LogOnline, Warning, TEXT("No default online subsystem."));
		}
	}
	else
	{
		UE_LOG(LogOnline, Warning, TEXT("No local player."));
	}
}

bool UDXCoreGISubsystem::CheckRequirableObject(UObject* object)
{
	if (object && object->IsValidLowLevel())
	{
		return object->GetClass()->ImplementsInterface(URequirable::StaticClass());
	}
	return false;
}

bool UDXCoreGISubsystem::CheckRequirement(FRequirement requirement)
{
	bool reqAlreadyConcluded = false;
	UWorld* world = GetWorld();
	UDXCoreWorldSubsystem* worldSubsystem = nullptr;
	if (world)
	{
		worldSubsystem = Cast<UDXCoreWorldSubsystem>(world->GetSubsystem<UDXCoreWorldSubsystem>());
	}

	const int selectDataKeeper = dataKeeper.HasData(requirement.RequiredDataName) ? 0 : ((worldSubsystem && worldSubsystem->dataKeeper.HasData(requirement.RequiredDataName)) ? 1 : 2);
	
	switch (selectDataKeeper) 
	{
	case 0:
		reqAlreadyConcluded |= dataKeeper.CheckRequirement(requirement);
		break;
	case 1:
		if (worldSubsystem)
		{
			reqAlreadyConcluded |= worldSubsystem->dataKeeper.CheckRequirement(requirement);
		}
		break;
	case 2:
		reqAlreadyConcluded |= _instantDataTemp.CheckRequirement(requirement);
		break;
	}
	
	return reqAlreadyConcluded;
}

void UDXCoreGISubsystem::BroadcastData(FName name)
{
	TMultiMap<FName, FRequirementRegister> excludeMap;

	TArray<FRequirementRegister*> registeredObjects;
	registeredRequirements.MultiFindPointer(name, registeredObjects, false);
	for (FRequirementRegister* r : registeredObjects)
	{
		if (!r->Continuous)
		{
			if (CheckRequirement(r->Requirement))
			{
				if (r->RegisteredObject->IsValidLowLevel() && !(r->RegisteredObject->HasAnyFlags(RF_BeginDestroyed) || r->RegisteredObject->HasAnyFlags(RF_FinishDestroyed)))
				{
					IRequirable::Execute_ReceiveGlobalRequirementConcluded(r->RegisteredObject, r->Requirement);
				}
				excludeMap.Add(name, *r);
			}
		}
		else
		{
			if (r->RegisteredObject->IsValidLowLevel() && !(r->RegisteredObject->HasAnyFlags(RF_BeginDestroyed) || r->RegisteredObject->HasAnyFlags(RF_FinishDestroyed)))
			{
				IRequirable::Execute_ReceiveGlobalRequirementChanged(r->RegisteredObject, r->Requirement, CheckRequirement(r->Requirement));
			}
		}
	}

	for (TPair<FName, FRequirementRegister> r : excludeMap)
	{
		registeredRequirements.Remove(r.Key, r.Value);
	}
}

void UDXCoreGISubsystem::RegisterForGlobalRequirement(const FRequirement requirement, UObject* object, bool continuous)
{
	UDXCoreWorldSubsystem* worldSubsystem = nullptr;
	UWorld* world = GetWorld();
	if (world)
	{
		worldSubsystem = Cast<UDXCoreWorldSubsystem>(world->GetSubsystem<UDXCoreWorldSubsystem>());
	}
	
	
	//If requirement is already met
	if (CheckRequirement(requirement))
	{
		if (CheckRequirableObject(object))
		{
			if (continuous) 
			{
				FRequirementRegister objReg = FRequirementRegister();
				objReg.RegisteredObject = object;
				objReg.Requirement = requirement;
				objReg.Continuous = continuous;
				registeredRequirements.Add(requirement.RequiredDataName, objReg);

				bool hasInitialData = false;
				hasInitialData |= dataKeeper.HasData(requirement.RequiredDataName);
				if (hasInitialData == false && worldSubsystem->IsValidLowLevel())
				{
					hasInitialData |= worldSubsystem->dataKeeper.HasData(requirement.RequiredDataName);
				}

				if (!hasInitialData) 
				{
					//if the code reaches here it means the req was met with null data (req is probably checking against default value), so lets add it
					switch ((requirement.Type)) 
					{
					case ERequirementType::AT_Bool:
						SendRequiredBool(requirement.RequiredDataName, false, EGlobalRequiredDataPersistance::GRDP_LevelPersist, false);
						break;
					case ERequirementType::AT_Numeric:
						SendRequiredInt(requirement.RequiredDataName, 0, EGlobalRequiredDataPersistance::GRDP_LevelPersist, false);
						break;
					case ERequirementType::AT_String:
						SendRequiredString(requirement.RequiredDataName, FString(""), EGlobalRequiredDataPersistance::GRDP_LevelPersist, false);
						break;
					default:
						break;
					}
					//No need to execute receiveRequirementChanged because SendRequired will already end up on a Broadcast.
				}
				else
				{
					//Make sure we let it know it's true when it was already met with valid data
					IRequirable::Execute_ReceiveGlobalRequirementChanged(object, requirement, true);
				}
			}
			else
			{
				IRequirable::Execute_ReceiveGlobalRequirementConcluded(object, requirement);
			}
			
		}
	}//OtherWise
	else
	{
		if (CheckRequirableObject(object))
		{
			FRequirementRegister objReg = FRequirementRegister();
			objReg.RegisteredObject = object;
			objReg.Requirement = requirement;
			objReg.Continuous = continuous;
			registeredRequirements.Add(requirement.RequiredDataName, objReg);
		}
	}
}

void UDXCoreGISubsystem::RemoveAllRegistersFrom(UObject* object)
{
	TMultiMap<FName, FRequirementRegister> excludeMap;
	for (TPair<FName, FRequirementRegister> r : registeredRequirements)
	{
		if (r.Value.RegisteredObject == object)
		{
			excludeMap.Add(r.Key, r.Value);
		}
	}

	for (TPair<FName, FRequirementRegister> r : excludeMap)
	{
		registeredRequirements.Remove(r.Key, r.Value);
	}
}

void UDXCoreGISubsystem::ClearNullRequirements()
{
	TMultiMap<FName, FRequirementRegister> excludeMap;
	for (TPair<FName, FRequirementRegister> r : registeredRequirements)
	{
		if (!r.Value.RegisteredObject)
		{
			excludeMap.Add(r.Key, r.Value);
		}
	}

	for (TPair<FName, FRequirementRegister> r : excludeMap)
	{
		registeredRequirements.Remove(r.Key, r.Value);
	}
}

void UDXCoreGISubsystem::ExportPersistentGlobalData(FGlobalDataKeeper& copy) const
{
	copy = dataKeeper;
	return;
}

void UDXCoreGISubsystem::SendRequiredIntByStruct(FIntGlobalData intGlobalDataStruct)
{
	SendRequiredInt(intGlobalDataStruct.RequiredName,  intGlobalDataStruct.NewValue, intGlobalDataStruct.DataPersistance, intGlobalDataStruct.Incremental);
}

void UDXCoreGISubsystem::SendRequiredFloatByStruct(FFloatGlobalData floatGlobalDataStruct)
{
	SendRequiredFloat(floatGlobalDataStruct.RequiredName,floatGlobalDataStruct.NewValue, floatGlobalDataStruct.DataPersistance, floatGlobalDataStruct.Incremental);
}

void UDXCoreGISubsystem::SendRequiredStringByStruct(FTextGlobalData textGlobalDataStruct)
{
	SendRequiredString(textGlobalDataStruct.RequiredName,textGlobalDataStruct.NewValue, textGlobalDataStruct.DataPersistance, textGlobalDataStruct.Incremental);
}

void UDXCoreGISubsystem::SendRequiredBoolByStruct(FBoolGlobalData boolGlobalDataStruct)
{
	SendRequiredBool(boolGlobalDataStruct.RequiredName,boolGlobalDataStruct.NewValue, boolGlobalDataStruct.DataPersistance, boolGlobalDataStruct.Incremental);
}

void UDXCoreGISubsystem::SendRequiredInt(const FName dataName, const int value,
                                         EGlobalRequiredDataPersistance persistence, const bool bIncremental)
{
	int broadcastValue = value;
	UWorld* world = GetWorld();
	UDXCoreWorldSubsystem* worldSubsystem = nullptr;
	if(world)
	{
		worldSubsystem = Cast<UDXCoreWorldSubsystem>(world->GetSubsystem<UDXCoreWorldSubsystem>());
	}
	
	switch (persistence)
	{
	case EGlobalRequiredDataPersistance::GRDP_Persist:
		if(worldSubsystem)
		{
			//ensure no duplicated dataName between persistence configurations (persist preferred over persistLevel)
			if(worldSubsystem->dataKeeper.HasData(dataName))
			{
				broadcastValue = worldSubsystem->dataKeeper.AddIntData(dataName, value, bIncremental);
				worldSubsystem->dataKeeper.RemoveData(dataName);
			}
		}
		broadcastValue = dataKeeper.AddIntData(dataName, broadcastValue, bIncremental);
		break;
	case EGlobalRequiredDataPersistance::GRDP_LevelPersist:
		//ensure no duplicated dataName between persistence configurations (persist preferred over persistLevel)
		if(!dataKeeper.HasData(dataName))
		{
			if (worldSubsystem)
			{
				broadcastValue = worldSubsystem->dataKeeper.AddIntData(dataName, value, bIncremental);
			}	
		}
		break;
	case EGlobalRequiredDataPersistance::GRDP_Instant:
		//Ensure instant data cannot contradict higher persistent data
		if(dataKeeper.HasData(dataName) || (worldSubsystem && worldSubsystem->dataKeeper.HasData(dataName)))
		{
			break;
		}
		_instantDataTemp.AddIntData(dataName, value, false); //ensure instant data is never incremental
		break;		
	}

	BroadcastData(dataName);

	_instantDataTemp.ClearData();
}

void UDXCoreGISubsystem::SendRequiredFloat(const FName dataName, const float value,
	EGlobalRequiredDataPersistance persistence, const bool bIncremental)
{
	float broadcastValue = value;
	UWorld* world = GetWorld();
	UDXCoreWorldSubsystem* worldSubsystem = nullptr;
	if(world)
	{
		worldSubsystem = Cast<UDXCoreWorldSubsystem>(world->GetSubsystem<UDXCoreWorldSubsystem>());
	}
	
	switch (persistence)
	{
	case EGlobalRequiredDataPersistance::GRDP_Persist:
		if(worldSubsystem)
		{
			//ensure no duplicated dataName between persistence configurations (persist preferred over persistLevel)
			if(worldSubsystem->dataKeeper.HasData(dataName))
			{
				broadcastValue = worldSubsystem->dataKeeper.AddFloatData(dataName, value, bIncremental);
				worldSubsystem->dataKeeper.RemoveData(dataName);
			}
		}
		broadcastValue = dataKeeper.AddFloatData(dataName, broadcastValue, bIncremental);
		break;
	case EGlobalRequiredDataPersistance::GRDP_LevelPersist:
		//ensure no duplicated dataName between persistence configurations (persist preferred over persistLevel)
		if(!dataKeeper.HasData(dataName))
		{
			if (worldSubsystem)
			{
				broadcastValue = worldSubsystem->dataKeeper.AddFloatData(dataName, value, bIncremental);
			}	
		}
		break;
	case EGlobalRequiredDataPersistance::GRDP_Instant:
		//Ensure instant data cannot contradict higher persistent data
		if(dataKeeper.HasData(dataName) || (worldSubsystem && worldSubsystem->dataKeeper.HasData(dataName)))
		{
			break;
		}
		_instantDataTemp.AddFloatData(dataName, value, false); //ensure instant data is never incremental
		break;		
	}

	BroadcastData(dataName);

	_instantDataTemp.ClearData();
}

void UDXCoreGISubsystem::SendRequiredString(const FName dataName, const FString value,
	EGlobalRequiredDataPersistance persistence, const bool bIncremental)
{
	FString broadcastValue = value;
	UWorld* world = GetWorld();
	UDXCoreWorldSubsystem* worldSubsystem = nullptr;
	if(world)
	{
		worldSubsystem = Cast<UDXCoreWorldSubsystem>(world->GetSubsystem<UDXCoreWorldSubsystem>());
	}
	
	switch (persistence)
	{
	case EGlobalRequiredDataPersistance::GRDP_Persist:
		if(worldSubsystem)
		{
			//ensure no duplicated dataName between persistence configurations (persist preferred over persistLevel)
			if(worldSubsystem->dataKeeper.HasData(dataName))
			{
				broadcastValue = worldSubsystem->dataKeeper.AddStringData(dataName, value, bIncremental);
				worldSubsystem->dataKeeper.RemoveData(dataName);
			}
		}
		broadcastValue = dataKeeper.AddStringData(dataName, broadcastValue, bIncremental);
		break;
	case EGlobalRequiredDataPersistance::GRDP_LevelPersist:
		//ensure no duplicated dataName between persistence configurations (persist preferred over persistLevel)
		if(!dataKeeper.HasData(dataName))
		{
			if (worldSubsystem)
			{
				broadcastValue = worldSubsystem->dataKeeper.AddStringData(dataName, value, bIncremental);
			}	
		}
		break;
	case EGlobalRequiredDataPersistance::GRDP_Instant:
		//Ensure instant data cannot contradict higher persistent data
		if(dataKeeper.HasData(dataName) || (worldSubsystem && worldSubsystem->dataKeeper.HasData(dataName)))
		{
			break;
		}
		_instantDataTemp.AddStringData(dataName, value, false); //ensure instant data is never incremental
		break;		
	}

	BroadcastData(dataName);

	_instantDataTemp.ClearData();
}

void UDXCoreGISubsystem::SendRequiredBool(const FName dataName, const bool value,
	EGlobalRequiredDataPersistance persistence, const bool bIncremental)
{
	UE_LOG(LogTemp, Warning, TEXT("Global data: %s was sent with %s"), *dataName.ToString(), value ? TEXT("true") : TEXT("false"));
	bool broadcastValue = value;
	UWorld* world = GetWorld();
	UDXCoreWorldSubsystem* worldSubsystem = nullptr;
	if(world)
	{
		worldSubsystem = Cast<UDXCoreWorldSubsystem>(world->GetSubsystem<UDXCoreWorldSubsystem>());
	}
	
	switch (persistence)
	{
	case EGlobalRequiredDataPersistance::GRDP_Persist:
		if(worldSubsystem)
		{
			//ensure no duplicated dataName between persistence configurations (persist preferred over persistLevel)
			if(worldSubsystem->dataKeeper.HasData(dataName))
			{
				broadcastValue = worldSubsystem->dataKeeper.AddBoolData(dataName, value, bIncremental);
				dataKeeper.RemoveData(dataName);
			}
		}
		broadcastValue = dataKeeper.AddBoolData(dataName, broadcastValue, bIncremental);
		break;
	case EGlobalRequiredDataPersistance::GRDP_LevelPersist:
		//ensure no duplicated dataName between persistence configurations (persist preferred over persistLevel)
		if(!dataKeeper.HasData(dataName))
		{
			if (worldSubsystem)
			{
				broadcastValue = worldSubsystem->dataKeeper.AddBoolData(dataName, value, bIncremental);
			}	
		}
		break;
	case EGlobalRequiredDataPersistance::GRDP_Instant:
		//Ensure instant data cannot contradict higher persistent data
		if(dataKeeper.HasData(dataName) || (worldSubsystem && worldSubsystem->dataKeeper.HasData(dataName)))
		{
			break;
		}
		_instantDataTemp.AddBoolData(dataName, value, false); //ensure instant data is never incremental
		break;		
	}

	BroadcastData(dataName);

	_instantDataTemp.ClearData();
}

const FString UDXCoreGISubsystem::MainSaveGameName = FString("MainSaveGame");

void UDXCoreGISubsystem::LoadMainSaveGame(const FMainSaveGameDiskOperation& PostLoadCallback)
{
	if(bDiskOperationInProgress)
	{
		UE_LOG(DXCore, Warning, TEXT("Tried loading MainSaveGame from disk but disk operation is already in progress."));
		OnLoadGame.Broadcast(false,nullptr);
		return;
	}
		
	if(bSaveSystemIsCorrupt)
	{
		UE_LOG(DXCore, Error, TEXT("Tried loading MainSaveGame from disk but Save System is corrupt."));
		OnLoadGame.Broadcast(false,nullptr);
		return;
	}

	const int32 PlatformID = GetPlayerPlatformID(GetActiveLocalPlayer());
	if(PlatformID < 0)
	{
		UE_LOG(DXCore, Error, TEXT("Tried loading MainSaveGame from disk but PlayerPlatformID is invalid."));
		// ReSharper disable once CppExpressionWithoutSideEffects
		PostLoadCallback.ExecuteIfBound(false);
		OnLoadGame.Broadcast(false,nullptr);
		return;	
	}
	
	if(!UGameplayStatics::DoesSaveGameExist(MainSaveGameName, PlatformID))
	{
		UE_LOG(DXCore, Warning, TEXT("Tried loading MainSaveGame from disk but disk file does not exist for this PlayerPlatformID."));
		// ReSharper disable once CppExpressionWithoutSideEffects
		PostLoadCallback.ExecuteIfBound(false);
	}

	bDiskOperationInProgress = true;
	UE_LOG(DXCore, Display, TEXT("Loading MainSaveGame from disk for PlayerPlatformID: %d."), PlatformID);
	UGameplayStatics::AsyncLoadGameFromSlot(MainSaveGameName, PlatformID, FAsyncLoadGameFromSlotDelegate::CreateLambda(
		[this, PostLoadCallback](const FString& slotName, const int32 userIndex, USaveGame* saveGame)
		{
			UDXCoreMainSaveGame* mainSaveGame = Cast<UDXCoreMainSaveGame>(saveGame);
			if(mainSaveGame && mainSaveGame->IsValidLowLevel())
			{
				UE_LOG(DXCore, Display, TEXT("MainSaveGame loaded from disk for PlayerPlatformID: %d."), userIndex);
				_mainSaveGame = mainSaveGame;
				_mainSaveGame->InitFromSubsystem(this, SaveGameTargetVersion);
				bDiskOperationInProgress = false;
				// ReSharper disable once CppExpressionWithoutSideEffects
				OnLoadGame.Broadcast(true,_mainSaveGame);
				PostLoadCallback.ExecuteIfBound(true);				
			}
			else
			{
				UE_LOG(DXCore, Error, TEXT("Tried loading MainSaveGame from disk but MainSaveGame was not valid or did not cast correctly."));
				bDiskOperationInProgress = false;
				// ReSharper disable once CppExpressionWithoutSideEffects
				PostLoadCallback.ExecuteIfBound(false);
				OnLoadGame.Broadcast(false,nullptr);
			}
		}));
		
}

UDXCoreMainSaveGame* UDXCoreGISubsystem::GetMainSaveGame(EGetSave& OutExecution)
{
	if(bDiskOperationInProgress)
	{
		OutExecution = EGetSave::DiskOperationInProgress;
		UE_LOG(DXCore, Error, TEXT("Tried accessing MainSaveGame but disk operation is currently in progress."));
		return nullptr;
	}
		
	if(bSaveSystemIsCorrupt)
	{
		OutExecution = EGetSave::Corruption;
		UE_LOG(DXCore, Error, TEXT("Tried accessing MainSaveGame but Save System is corrupted."));
		return nullptr;
	}

	const int32 PlatformID = GetPlayerPlatformID(GetActiveLocalPlayer());
	if(PlatformID < 0)
	{
		OutExecution = EGetSave::UserInvalid;
		UE_LOG(DXCore, Error, TEXT("Tried accessing MainSaveGame but PlayerPlatformID is invalid."));
		return nullptr;	
	}

	if(_mainSaveGame && _mainSaveGame->IsValidLowLevel())
	{
		if(_mainSaveGame->IsSaveGameCorrupt())
		{
			OutExecution = EGetSave::Corruption;
			UE_LOG(DXCore, Error, TEXT("Tried accessing MainSaveGame but SaveGame is corrupted."));
			return nullptr;
		}
		else
		{
			//Set Execution to Then since it was accessed correctly :)
			OutExecution = EGetSave::Then;
		}
	}
	else if(UGameplayStatics::DoesSaveGameExist(MainSaveGameName, PlatformID))
	{
		OutExecution = EGetSave::NotLoaded;
		UE_LOG(DXCore, Display, TEXT("Tried accessing MainSaveGame, which is null and has file on disk, so should be loaded first."));
		return nullptr;
	}
	else
	{
		UDXCoreMainSaveGame* mainSave = Cast<UDXCoreMainSaveGame>(UGameplayStatics::CreateSaveGameObject(MainSaveGameClass.Get()));
		if(mainSave)
		{
			//Set Execution to new save, since we are first creating a new save and then returning it by the end of the function
			OutExecution = EGetSave::NewSave;
			_mainSaveGame = mainSave;
			_mainSaveGame->InitFromSubsystem(this, SaveGameTargetVersion);
		}
		else
		{
			OutExecution = EGetSave::Corruption;
			UE_LOG(DXCore, Error, TEXT("Tried accessing MainSaveGame by creating new one, but class was invalid or instance corrupted."));
			return nullptr;
		}
	}

	return _mainSaveGame;

}

void UDXCoreGISubsystem::SaveMainSaveGame(const FMainSaveGameDiskOperation& PostSaveCallback)
{
	if(bDiskOperationInProgress)
	{
		UE_LOG(DXCore, Warning, TEXT("Tried saving MainSaveGame to disk but disk operation is already in progress."));
		return;
	}
		
	if(bSaveSystemIsCorrupt)
	{
		UE_LOG(DXCore, Error, TEXT("Tried saving MainSaveGame to disk but Save System is corrupted."));
		return;
	}

	if(!(_mainSaveGame && _mainSaveGame->IsValidLowLevel()))
	{
		UE_LOG(DXCore, Warning, TEXT("Tried saving MainSaveGame to disk but the memory instance was null or invalid."));
		// ReSharper disable once CppExpressionWithoutSideEffects
		PostSaveCallback.ExecuteIfBound(false);
		return;
	}

	if(_mainSaveGame->IsSaveGameCorrupt())
	{
		UE_LOG(DXCore, Error, TEXT("Tried saving MainSaveGame to disk but the memory instance was corrupted."));
		// ReSharper disable once CppExpressionWithoutSideEffects
		PostSaveCallback.ExecuteIfBound(false);
		return;
	}

	const int32 PlatformID = GetPlayerPlatformID(GetActiveLocalPlayer());
	if(PlatformID < 0)
	{
		UE_LOG(DXCore, Error, TEXT("Tried saving MainSaveGame to disk but PlayerPlatformID was invalid."));
		// ReSharper disable once CppExpressionWithoutSideEffects
		PostSaveCallback.ExecuteIfBound(false);
		return;	
	}

	bDiskOperationInProgress = true;
	OnPreSaveGame.Broadcast(MainSaveGameName);
	UE_LOG(DXCore, Display, TEXT("Saving MainSaveGame to disk for PlayerPlatformID: %d."), PlatformID);
	UGameplayStatics::AsyncSaveGameToSlot(_mainSaveGame, MainSaveGameName, PlatformID, FAsyncSaveGameToSlotDelegate::CreateLambda(
		[this, PostSaveCallback](const FString& slotName, const int32 userIndex, bool success)
		{
			if(success)
			{
				UE_LOG(DXCore, Display, TEXT("MainSaveGame saved to disk for PlayerPlatformID: %d."), userIndex);
				bDiskOperationInProgress = false;
				// ReSharper disable once CppExpressionWithoutSideEffects
				PostSaveCallback.ExecuteIfBound(true);
			}
			else
			{
				UE_LOG(DXCore, Error, TEXT("Tried saving MainSaveGame to disk but failed."));
				bDiskOperationInProgress = false;
				// ReSharper disable once CppExpressionWithoutSideEffects
				PostSaveCallback.ExecuteIfBound(false);
			}
			OnPostSaveGame.Broadcast(slotName);
		}));
}

void UDXCoreGISubsystem::UnloadMainSaveGame()
{
	_mainSaveGame = nullptr;
}

bool UDXCoreGISubsystem::DeleteMainSaveGame()
{
	if(bDiskOperationInProgress)
	{
		UE_LOG(DXCore, Warning, TEXT("Tried deleting MainSaveGame from disk but disk operation is already in progress."));
		return false;
	}
		
	if(bSaveSystemIsCorrupt)
	{
		UE_LOG(DXCore, Error, TEXT("Tried deleting MainSaveGame from disk but Save System is corrupted."));
		return false;
	}

	if(_mainSaveGame && _mainSaveGame->IsValidLowLevel())
	{
		if(_mainSaveGame->IsSaveGameCorrupt())
		{
			UE_LOG(DXCore, Error, TEXT("Tried deleting MainSaveGame from disk but the memory instance was corrupted, cancelling the operation."));
			return false;
		}
	}

	const int32 PlatformID = GetPlayerPlatformID(GetActiveLocalPlayer());
	if(PlatformID < 0)
	{
		UE_LOG(DXCore, Error, TEXT("Tried deleting MainSaveGame from disk but PlayerPlatformID was invalid."));
		return false;
	}

	const bool saveGameExisted = UGameplayStatics::DoesSaveGameExist(MainSaveGameName, PlatformID);
	if(!saveGameExisted)
	{
		UE_LOG(DXCore, Warning, TEXT("Tried deleting MainSaveGame from disk but disk file did not exist for this PlayerPlatformID: %d."), PlatformID);
		return false;
	}

	if(saveGameExisted)
	{
		bDiskOperationInProgress = true;
		UE_LOG(DXCore, Warning, TEXT("Deleting MainSaveGame from disk for this PlayerPlatformID: %d."), PlatformID);
		const bool deletedSaveGame = UGameplayStatics::DeleteGameInSlot(MainSaveGameName, PlatformID);
		bDiskOperationInProgress = false;
		UE_LOG(DXCore, Warning, TEXT("MainSaveGame deleted from disk for this PlayerPlatformID: %d."), PlatformID);
		if(deletedSaveGame)
		{
			UnloadMainSaveGame();
			return true;
		}
		UE_LOG(DXCore, Error, TEXT("Tried deleting MainSaveGame from disk but failed for this PlayerPlatformID: %d."), PlatformID);
		return false;
	}

	UE_LOG(DXCore, Warning, TEXT("Tried deleting MainSaveGame from disk for PlayerPlatformID %d but no file existed so only clearing memory-instance"), PlatformID);
	UnloadMainSaveGame();
	return true;
}

UTexture2D *UDXCoreGISubsystem::GetSteamAvatar(APlayerState* ps)
{
/*#if PLATFORM_WINDOWS

	if(!ps)
		return nullptr;

	//ensure we are using steam
	const IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get("Steam");
	if(!onlineSubsystem)
		return nullptr;
	
	if (SteamAPI_Init())
	{
		//steam id is the ps player id if using subsystem steam
		const FUniqueNetIdRepl steamId = ps->GetUniqueId();
		CSteamID PlayerRawID = SteamIDStringToCSteamID(steamId.ToString());
		
		uint32 Width;
		uint32 Height;
		
		//Getting the PictureID from the SteamAPI and getting the Size with the ID
		int Picture = SteamFriends()->GetMediumFriendAvatar(PlayerRawID);
		SteamUtils()->GetImageSize(Picture, &Width, &Height);


		if (Width > 0 && Height > 0)
		{
			//Creating the buffer "oAvatarRGBA" and then filling it with the RGBA Stream from the Steam Avatar
			unsigned char *oAvatarRGBA = new unsigned char[Width * Height * 4];


			//Filling the buffer with the RGBA Stream from the Steam Avatar and creating a UTextur2D to parse the RGBA Steam in
			SteamUtils()->GetImageRGBA(Picture, (uint8*)oAvatarRGBA, 4 * Height * Width * sizeof(char));

			//Swap R and B channels because for some reason the games whack
			for (uint32 i = 0; i < (Width * Height * 4); i += 4)
			{
				uint8 Temp = oAvatarRGBA[i + 0];
				oAvatarRGBA[i + 0] = oAvatarRGBA[i + 2];
				oAvatarRGBA[i + 2] = Temp;
			}

			UTexture2D* Avatar = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);

			//MAGIC!
			uint8* MipData = (uint8*)Avatar->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(MipData, (void*)oAvatarRGBA, Height * Width * 4);
			Avatar->GetPlatformData()->Mips[0].BulkData.Unlock();

			//Setting some Parameters for the Texture and finally returning it
			Avatar->GetPlatformData()->SetNumSlices(1);
			Avatar->NeverStream = true;
			//Avatar->CompressionSettings = TC_EditorIcon;

			Avatar->UpdateResource();

			return Avatar;
		}
		return nullptr;
	}
#endif*/
	return nullptr;
}
