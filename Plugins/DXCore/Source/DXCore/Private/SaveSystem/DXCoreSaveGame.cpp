// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem/DXCoreSaveGame.h"

#include "Core/DXCoreGISubsystem.h"
#include "Core/DXCoreSettings.h"
#include "Kismet/GameplayStatics.h"


void UDXCoreSlotSaveGame::FillSaveInformation_Implementation(UObject* WorldContextObject)
{
	UDXCoreGISubsystem* GISubsystem = WorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UDXCoreGISubsystem>();
	if(GISubsystem)
	{
		GISubsystem->ExportPersistentGlobalData(persistentData);
	}
}

void UDXCoreSlotSaveGame::BroadcastSaveInformation_Implementation(UObject* WorldContextObject)
{
}

UDXCoreGISubsystem* UDXCoreMainSaveGame::GetGISubsystem() const
{
	return _giSubsystem;
}

bool UDXCoreMainSaveGame::HandleSaveGameTargetVersionMismatch_Implementation(const FString& InLastSavedTargetVersion,
	const FString& InNewTargetVersion)
{
	if(InLastSavedTargetVersion == InNewTargetVersion)
	{
		return true;
	}
	return false;
}


void UDXCoreMainSaveGame::InitFromSubsystem(UDXCoreGISubsystem* GISubsystem, FString CurrentSaveGameTargetVersion)
{
	_giSubsystem = GISubsystem;

	bSaveSystemIsCorrupt = GISubsystem->IsSaveSystemCorrupt();

	//Check first if this is not a new save
	if(SaveGameTargetVersion.IsEmpty())
	{
		SaveGameTargetVersion = CurrentSaveGameTargetVersion;	
	}
	else
	{
		if(SaveGameTargetVersion != CurrentSaveGameTargetVersion)
		{
			const bool successfullyConverted = HandleSaveGameTargetVersionMismatch(SaveGameTargetVersion, CurrentSaveGameTargetVersion);

			if(successfullyConverted)
			{
				SaveGameTargetVersion = CurrentSaveGameTargetVersion;
			}
			else
			{
				bSaveGameIsCorrupt = true;
			}
		}
	}
	//Make sure no old pointers are loaded
	//_loadedGameSlots.Empty(); //No longer needed, made variable Transient
}

bool UDXCoreMainSaveGame::HasGameSlot(FString SlotName) const
{
	//Don't bother looking for GameSlots if SlotInfo is empty.
	if(_gameSlotsInfo.IsEmpty())
		return false;
	
	return _gameSlotsInfo.Contains(SlotName);
}

//Todo: Test if this return will allow programmers to change the content of Info in Blueprints
void UDXCoreMainSaveGame::GetGameSlotInfo(FString SlotName, FParameterKeeperData& OutInfo)
{
	if(HasGameSlot(SlotName))
	{
		OutInfo = _gameSlotsInfo[SlotName];
	}
}

void UDXCoreMainSaveGame::SetGameSlotInfo(FString SlotName, const FParameterKeeperData& InInfo)
{
	if(HasGameSlot(SlotName))
	{
		_gameSlotsInfo[SlotName] = InInfo;
	}
}

bool UDXCoreMainSaveGame::IsGameSlotLoaded(FString SlotName)
{
	return HasGameSlot(SlotName) && _loadedGameSlots.Contains(SlotName) && _loadedGameSlots[SlotName] && _loadedGameSlots[SlotName]->IsValidLowLevel();
}

bool UDXCoreMainSaveGame::CreateGameSlot(FString SlotName, FParameterKeeperData SlotInfo)
{
	if(bSaveGameIsCorrupt || bSaveSystemIsCorrupt)
		return false;
	
	//Make sure no "none" or "" game slots can be made
	if(SlotName.IsEmpty())
		return false;

	//If this game slot name already exists it must be deleted/renamed before it can be created again.
	//To access an already created slot from another session it must be loaded instead of created
	if(HasGameSlot(SlotName))
		return false;

	const UDXCoreSettings* settings = GetDefault<UDXCoreSettings>();
	if(!(settings && settings->IsValidLowLevel()))
		return false;

	UDXCoreSlotSaveGame* newGameSlot = Cast<UDXCoreSlotSaveGame>(UGameplayStatics::CreateSaveGameObject(settings->SaveGameVersions[SaveGameTargetVersion].SlotSaveGameClass.Get()));
	if(!(newGameSlot && newGameSlot->IsValidLowLevel()))
		return false;
	
	_gameSlotsInfo.Add(SlotName, SlotInfo);
	_loadedGameSlots.Add(SlotName, newGameSlot);
	
	return true;
}

bool UDXCoreMainSaveGame::UnloadGameSlot(FString SlotName)
{
	if(IsGameSlotLoaded(SlotName))
	{
		return _loadedGameSlots.Remove(SlotName) > 0;
	}
	return false;
}

bool UDXCoreMainSaveGame::DeleteGameSlot(FString SlotName)
{
	if(bSaveGameIsCorrupt || bSaveSystemIsCorrupt)
		return false;

	//Make sure no "none" or "" game slots can be made
	if(SlotName.IsEmpty())
		return false;

	//If this game slot name already exists it must be deleted/renamed before it can be created again.
	//To access an already created slot from another session it must be loaded instead of created
	if(!HasGameSlot(SlotName))
		return false;
	
	const int32 PlatformID = GetGISubsystem()->GetPlayerPlatformID(GetGISubsystem()->GetActiveLocalPlayer());
	//Check if player is valid for this operation
	if(PlatformID < 0)
		return false;

	const bool wasLoaded = IsGameSlotLoaded(SlotName);

	const bool DiskExisted = UGameplayStatics::DoesSaveGameExist(SlotName, PlatformID);
	bool DiskDeleted = false;
	if(DiskExisted)
	{
		DiskDeleted = UGameplayStatics::DeleteGameInSlot(SlotName, PlatformID);
	}

	if(DiskExisted && DiskDeleted == false)
		return false;

	if(wasLoaded)
	{
		bool unloaded = UnloadGameSlot(SlotName);
		if(DiskExisted && DiskDeleted && unloaded == false)
		{
			//UE_LOG(LogTemp, Error, TEXT("Trying to delete gameslot %, disk existed and was deleted and was loaded but was unable to unload from memory for some reason. GameSlotsInfo will be removed anyway."), SlotName);
		}
		else if(DiskExisted == false)
		{
			//Since disk did not exist and a loaded save could not be removed from loaded list, cancel operation
			return false;
		}
	}

	//Finally remove from GameSlotsInfo
	_gameSlotsInfo.Remove(SlotName);

	return true;

}

bool UDXCoreMainSaveGame::SetActiveGameSlot(FString SlotName)
{
	if(SlotName.IsEmpty())
		return false;
	
	if(!HasGameSlot(SlotName))
		return false;

	if(!IsGameSlotLoaded(SlotName))
		return false;

	ActiveGameSlot = SlotName;
	return true;
}

void UDXCoreMainSaveGame::LoadGameSlot(FString SlotName, const FSaveGameDiskOperationCallback& PostLoadCallback)
{
	//If another task is already in place or save is corrupt just ignore, do not send callback as failed.
	if(bDiskOperationInProgress || bSaveGameIsCorrupt || bSaveSystemIsCorrupt)
		return;

	//Need to check if this slot was ever created before. A Slot and SlotInfo must be created before trying to load 
	//If it was this session it should be here, if in other session, the MainSaveGame should have been loaded with this info.
	if(!HasGameSlot(SlotName))
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		PostLoadCallback.ExecuteIfBound(SlotName, false);
		return;
	}

	const int32 PlatformID = GetGISubsystem()->GetPlayerPlatformID(GetGISubsystem()->GetActiveLocalPlayer());
	//Check if player is valid for this operation
	if(PlatformID < 0)
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		PostLoadCallback.ExecuteIfBound(SlotName, false);
		return;
	}

	//If savegame does not exist in disk, do not try to load it
	if(!UGameplayStatics::DoesSaveGameExist(SlotName, PlatformID))
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		PostLoadCallback.ExecuteIfBound(SlotName, false);
		return;
	}
		

	bDiskOperationInProgress = true;
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, PlatformID, FAsyncLoadGameFromSlotDelegate::CreateLambda([this,PostLoadCallback](const FString& slotName, const int32 playerID, USaveGame* savegame)
	{
		UDXCoreSlotSaveGame* _saveGame = Cast<UDXCoreSlotSaveGame>(savegame);
		if(_saveGame)
		{				
			if(_loadedGameSlots.Contains(slotName))
			{
				_loadedGameSlots[slotName] = _saveGame;
			}
			else
			{
				_loadedGameSlots.Add(slotName, _saveGame);
			}
			bDiskOperationInProgress = false;
			// ReSharper disable once CppExpressionWithoutSideEffects
			PostLoadCallback.ExecuteIfBound(slotName, true);
		}
		else
		{
			bDiskOperationInProgress = false;
			// ReSharper disable once CppExpressionWithoutSideEffects
			PostLoadCallback.ExecuteIfBound(slotName, false);
		}
	}));
	
}

UDXCoreSlotSaveGame* UDXCoreMainSaveGame::GetGameSlot(FString SlotName)
{
	if(IsGameSlotLoaded(SlotName))
	{
		return _loadedGameSlots[SlotName];
	}
	return nullptr;
}

void UDXCoreMainSaveGame::SaveGameSlot(FString SlotName, const FSaveGameDiskOperationCallback& PostSaveCallback)
{
	//If another task is already in place just ignore or save is corrupt, do not send callback as failed
	if(bDiskOperationInProgress || bSaveGameIsCorrupt || bSaveSystemIsCorrupt)
		return;

	//Need to check if this slot was ever created before. A Slot and SlotInfo must be created before trying to save 
	//If it was this session it should be here, if in other session, the MainSaveGame should have been loaded with this info.
	if(!HasGameSlot(SlotName))
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		PostSaveCallback.ExecuteIfBound(SlotName, false);
		return;
	}

	//If gameslot object exists in GameSlotsInfo but is null, it was probably not loaded or created so it cannot be saved
	UDXCoreSlotSaveGame* gameSlot = GetGameSlot(SlotName);
	if(!(gameSlot && gameSlot->IsValidLowLevel()))
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		PostSaveCallback.ExecuteIfBound(SlotName, false);
		return;
	}

	const int32 PlatformID = GetGISubsystem()->GetPlayerPlatformID(GetGISubsystem()->GetActiveLocalPlayer());
	//Check if player is valid for this operation
	if(PlatformID < 0)
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		PostSaveCallback.ExecuteIfBound(SlotName, false);
		return;
	}

	bDiskOperationInProgress = true;
	GetGISubsystem()->OnPreSaveGame.Broadcast(SlotName);
	UGameplayStatics::AsyncSaveGameToSlot(gameSlot, SlotName, PlatformID, FAsyncSaveGameToSlotDelegate::CreateLambda([this, PostSaveCallback](const FString& slotName, const int32 playerID, bool success)
	{
		bDiskOperationInProgress = false;
		// ReSharper disable once CppExpressionWithoutSideEffects
		PostSaveCallback.ExecuteIfBound(slotName, true);
		GetGISubsystem()->OnPostSaveGame.Broadcast(slotName);
	}));
}

bool UDXCoreMainSaveGame::CopyGameSlot(FString SourceGameSlot, FString DestinationGameSlot)
{
	if(bSaveGameIsCorrupt || bSaveSystemIsCorrupt)
		return false;

	if(SourceGameSlot.IsEmpty() || DestinationGameSlot.IsEmpty())
		return false;
	
	if(!(HasGameSlot(SourceGameSlot) && HasGameSlot(DestinationGameSlot)))
		return false;

	if(!IsGameSlotLoaded(SourceGameSlot))
		return false;

	UDXCoreSlotSaveGame* duplicatedGameSlot = DuplicateObject(_loadedGameSlots[SourceGameSlot], _loadedGameSlots[SourceGameSlot]->GetOuter(), FName(DestinationGameSlot));

	if(!(duplicatedGameSlot && duplicatedGameSlot->IsValidLowLevel()))
		return false;
	
	_loadedGameSlots[DestinationGameSlot] = duplicatedGameSlot;
	return true;
	
}

UDXCoreSlotSaveGame* UDXCoreMainSaveGame::GetActiveGameSlot()
{
	return GetGameSlot(ActiveGameSlot);
}

void UDXCoreMainSaveGame::SaveActiveGameSlot(const FSaveGameDiskOperationCallback& PostSaveCallback)
{
	if(bSaveGameIsCorrupt || bSaveSystemIsCorrupt)
		return;

	SaveGameSlot(ActiveGameSlot, PostSaveCallback);
}
