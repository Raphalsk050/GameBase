// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/SaveGame.h"
#include "DXCoreImportantTypes.h"
//#include "Core/DXCoreGISubsystem.h"

#include "DXCoreSaveGame.generated.h"

class UDXCoreGISubsystem;
DECLARE_DYNAMIC_DELEGATE_TwoParams(FSaveGameDiskOperationCallback, FString, SlotName, bool, Success);

UCLASS()
class DXCORE_API UDXCoreSlotSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DXCore|Save System|Slot")
	FGlobalDataKeeper persistentData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DXCore|Save System|Slot")
	TMap<FName, FParameterKeeperData> actorSavedStates;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="DXCore|Save System|Slot", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	void FillSaveInformation(UObject* WorldContextObject);
	virtual void FillSaveInformation_Implementation(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintNativeEvent, Category="DXCore|Save System|Slot", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	void BroadcastSaveInformation(UObject* WorldContextObject);
	virtual void BroadcastSaveInformation_Implementation(UObject* WorldContextObject);
};

UCLASS()
class DXCORE_API UDXCoreMainSaveGame : public USaveGame
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere)
	TMap<FString, FParameterKeeperData> _gameSlotsInfo;
	
	UPROPERTY(VisibleAnywhere, Transient)
	TMap<FString, UDXCoreSlotSaveGame*> _loadedGameSlots;

	UPROPERTY(Transient)
	UDXCoreGISubsystem* _giSubsystem;

	bool bDiskOperationInProgress = false;

	UPROPERTY(VisibleAnywhere)
	FString SaveGameTargetVersion;

	FString _tempNewSaveGameTargetVersion;

	bool bSaveGameIsCorrupt = false;
	bool bSaveSystemIsCorrupt = false;
	
protected:

	UFUNCTION(BlueprintPure, Category = "DXCore|Save System|Main Save")
	bool IsSaveSystemCorrupt() const { return bSaveSystemIsCorrupt;}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DXCore|Save System|Main Save")
	FString ActiveGameSlot;

	UFUNCTION(BlueprintPure, Category = "DXCore|Save System|Main Save")
	UDXCoreGISubsystem* GetGISubsystem() const;

	/*Handle any mismatch in versions from the saved version to a newer version, maybe using different classes now
	It's important that any necessary conversion is made here before returning true and saving to disk again as
	saving it again	will update its SaveGameTargetVersion to the new one. Returning it as false will indicate that
	the conversion was not made and will prevent the SaveGameTargetVersion from updating and mark the savegame as
	corrupted which will prevent any further disk operations on slots.*/
	UFUNCTION(BlueprintNativeEvent, Category = "DXCore|Save System|Main Save")
	bool HandleSaveGameTargetVersionMismatch(const FString& InLastSavedTargetVersion, const FString& InNewTargetVersion);
	virtual bool HandleSaveGameTargetVersionMismatch_Implementation(const FString& InLastSavedTargetVersion, const FString& InNewTargetVersion);
public:

	void InitFromSubsystem(UDXCoreGISubsystem* GISubsystem, FString CurrentSaveGameTargetVersion);

	UFUNCTION(BlueprintPure, Category = "DXCore|Save System|Main Save")
	FString GetSaveGameTargetVersion() const { return SaveGameTargetVersion; }
	UFUNCTION(BlueprintPure, Category = "DXCore|Save System|Main Save")
	bool IsSaveGameCorrupt() const { return bSaveGameIsCorrupt;}
	
	UFUNCTION(BlueprintPure, Category = "DXCore|Save System|Main Save")
	bool HasGameSlot(FString SlotName) const;
	//Get GameSlot Info without loading the slot.
	UFUNCTION(BlueprintPure, Category = "DXCore|Save System|Main Save")
	void GetGameSlotInfo(FString SlotName, FParameterKeeperData& OutInfo);
	UFUNCTION(BlueprintCallable, Category = "DXCore|Save System|Main Save")
	void SetGameSlotInfo(FString SlotName, const FParameterKeeperData& InInfo);

	UFUNCTION(BlueprintPure, Category = "DXCore|Save System|Main Save")
	bool IsGameSlotLoaded(FString SlotName);

	//Create new game slot (without saving to disk). To override an existing one it must first be deleted/renamed
	UFUNCTION(BlueprintCallable, Category = "DXCore|Save System|Main Save")
	bool CreateGameSlot(FString SlotName, FParameterKeeperData SlotInfo);
	/*Unloads a game slot by releasing the pointer. Be careful when unloading active game slots, if this
	pointer is stored by another UPROPERTY the garbage collector won't remove it from memory.*/
	UFUNCTION(BlueprintCallable, Category = "DXCore|Save System|Main Save")
	bool UnloadGameSlot(FString SlotName);
	//Delete game slot both in memory and disk, if has disk file and deletion fails it will not delete in memory.
	UFUNCTION(BlueprintCallable, Category = "DXCore|Save System|Main Save")
	bool DeleteGameSlot(FString SlotName);
	//Set active game slot by Name (Must be loaded/created first)
	UFUNCTION(BlueprintCallable, Category = "DXCore|Save System|Main Save")
	bool SetActiveGameSlot(FString SlotName);

	//====== Use With Caution - The idea is to avoid keeping multiple game slots loaded and using SlotInfo instead
	
	//Load specific slot from disk, only works if it was ever created first (avoid keeping multiple game slots loaded, use SlotInfo instead)
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm = "PostLoadCallback"), Category = "DXCore|Save System|Main Save")
	void LoadGameSlot(FString SlotName, const FSaveGameDiskOperationCallback& PostLoadCallback);
	//Gets the game slot by name (Must be loaded/created first or else it will be null)
	UFUNCTION(BlueprintPure, Category = "DXCore|Save System|Main Save")
	UDXCoreSlotSaveGame* GetGameSlot(FString SlotName);
	//Saves specified slot to disk
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm = "PostSaveCallback"), Category = "DXCore|Save System|Main Save")
	void SaveGameSlot(FString SlotName, const FSaveGameDiskOperationCallback& PostSaveCallback);
	//Tries to copy from loaded game slot to another already existing game slot
	UFUNCTION(BlueprintCallable, Category = "DXCore|Save System|Main Save")
	bool CopyGameSlot(FString SourceGameSlot, FString DestinationGameSlot);
	
	//======
	
	//Get current active game slot
	UFUNCTION(BlueprintPure, Category = "DXCore|Save System|Main Save")
	UDXCoreSlotSaveGame* GetActiveGameSlot();
	//Save current active game slot
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm = "PostSaveCallback"), Category = "DXCore|Save System|Main Save")
	void SaveActiveGameSlot(const FSaveGameDiskOperationCallback& PostSaveCallback);
	
};
