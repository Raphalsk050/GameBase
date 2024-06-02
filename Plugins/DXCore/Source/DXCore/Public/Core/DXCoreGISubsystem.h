// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"

#include "DXCore.h"
#include "DXCoreImportantTypes.h"
#include "CosmeticEventSystem/CosmeticEventManager.h"
#include "SaveSystem/DXCoreSaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DXCoreGISubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAroundSaveGameOperation, FString, SlotName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAroundLoadGameOperation, bool, Success, UDXCoreMainSaveGame*, MainSaveGame);
DECLARE_DYNAMIC_DELEGATE_OneParam(FMainSaveGameDiskOperation, bool, Success);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelLoadChanged, FName, LevelName, bool, loaded);
/**
 * 
 */
UCLASS()
class DXCORE_API UDXCoreGISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(BlueprintAssignable, Category = "DXCore|LevelStreaming")
	FOnLevelLoadChanged OnLevelLoadChanged;

//==== PlayState and Level structuring code (_Game, _Env, etc.)
private:
	double _gameTime;
	FTSTicker::FDelegateHandle TickerDelegateHandle;
	bool TickDelegate(float DeltaTime);
	
	UPROPERTY()
	UWorld* currentWorld;
	void LevelAdded(ULevel* level, UWorld* world);
	void LevelRemoved(ULevel* level, UWorld* world);
	
	static const FName VisualBlockingLayerName;

	void UpdateLevelActors(ULevel* level, UWorld* world, FString levelName) const;

public:
	UFUNCTION(BlueprintPure, Category = "DXCore|Levels")
	static FName GetVisualBlockingLayerName(){ return VisualBlockingLayerName; }

	double GetGameTime() const { return _gameTime; }
//====

//====Active-User

protected:
	
	UPROPERTY()
	ULocalPlayer* _activeLocalPlayer;
	UPROPERTY()
	ULocalPlayer* _lastActiveLocalPlayer;

	//Use to reset save (BPs should listen to OnUserReset and send back to menu level etc.)
	UFUNCTION()
	void OnReceiveUserReset();
	
public:

	void SetActiveLocalPlayer(ULocalPlayer* newActiveLocalPlayer);
	ULocalPlayer* GetActiveLocalPlayer();
	ULocalPlayer* GetLastActiveLocalPlayer();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DXCore|App")
	bool bPreventMultipleInstances;

	//---Logic Config (Should be simply set to true when reaching the "Press any key to Start")
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DXCore|Active-User")
	bool bCanLookForFirstPlayerInput = false;

	//---Logic Delegates
	//Can be used to know when we should go back to the menu and reload any saves.
	UPROPERTY(BlueprintAssignable, Category = "DXCore|Active-User")
	FOnUserReset OnUserReset;
	//Can be used to know when the user is valid for the first time and should load any saves.
	UPROPERTY(BlueprintAssignable, Category = "DXCore|Active-User")
	FOnUserReset OnUserSet;

	//---Cosmetic Delegates - For showing disconnection UI and hiding it again
	UPROPERTY(BlueprintAssignable, Category = "DXCore|Active-User")
	FOnActiveControllerConnectionChanged OnActiveControllerDisconnected;
	UPROPERTY(BlueprintAssignable, Category = "DXCore|Active-User")
	FOnActiveControllerConnectionChanged OnActiveControllerDisconnectionSolved;

	//Can be used to know when we should go back to the menu and reload any saves.
	UFUNCTION(BlueprintPure, Category = "DXCore|Active-User")
	APlayerController* GetActivePlayerController() const;

	UFUNCTION(BlueprintPure, Category = "DXCore|Active-User")
	int32 GetPlayerPlatformID(ULocalPlayer* localPlayer);

	TSharedPtr<const FUniqueNetId> GetPlayerUniqueNetId(ULocalPlayer* localPlayer);

	//Can be used to trigger user change for the active user.
	UFUNCTION(BlueprintCallable, Category = "DXCore|Active-User")
	void ShowLoginUIForActiveController();
//====
	
//====Global Requirements and Data

private:
	//dataKeeper keeps all the persistent data received;
	FGlobalDataKeeper dataKeeper;
	//temp datakeeper just to facilitate instant data logic
	FGlobalDataKeeper _instantDataTemp;

	//registeredRequirements keeps all active registered requirements
	TMultiMap<FName, FRequirementRegister> registeredRequirements;

	//Check if object is valid and requirable
	bool CheckRequirableObject(UObject* object);
	//Check for requirement state either met(concluded) or not
	bool CheckRequirement(FRequirement requirement);
	
	/**
	* Compares every register with this name, execute the interface call and remove it from the register list
	* if Requirement register is not continuous.
	*	Name:  is the RequiredDataName of the requirement;*/
	void BroadcastData(FName name);

public:
	UFUNCTION(BlueprintCallable, Category = "DXCore|Requirement System")
	void RegisterForGlobalRequirement(const FRequirement requirement, UObject* object, bool continuous);
	UFUNCTION(BlueprintCallable, Category = "DXCore|Requirement System")
	void RemoveAllRegistersFrom(UObject* object);

	UFUNCTION(BlueprintCallable, Category = "DXCore|Requirement System")
	void ClearNullRequirements();

	void ExportPersistentGlobalData(FGlobalDataKeeper& copy) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DXCore|Requirement System")
	void SendRequiredInt(const FName dataName, const int value, EGlobalRequiredDataPersistance persistence, const bool bIncremental);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DXCore|Requirement System")
	void SendRequiredFloat(const FName dataName, const float value, EGlobalRequiredDataPersistance persistence, const bool bIncremental);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DXCore|Requirement System")
	void SendRequiredString(const FName dataName, const FString value, EGlobalRequiredDataPersistance persistence, const bool bIncremental);
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DXCore|Requirement System")
	void SendRequiredBool(const FName dataName, const bool value, EGlobalRequiredDataPersistance persistence, const bool bIncremental);

	//Functions for sending FGlobalData structs without having to break them. These call their matching SendRequired functions above.	
	//Sends a Required Int using a FIntGlobalData struct. Calls SendRequiredInt under the hood.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DXCore|Requirement System")	
	void SendRequiredIntByStruct(FIntGlobalData intGlobalDataStruct);
	
	//Sends a Required Float using a FFloatGlobalData struct. Calls SendRequiredFloat under the hood.	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DXCore|Requirement System")
	void SendRequiredFloatByStruct(FFloatGlobalData floatGlobalDataStruct);

	//Sends a Required Text using a FTextGlobalData struct. Calls SendRequiredText under the hood.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DXCore|Requirement System")
	void SendRequiredStringByStruct(FTextGlobalData textGlobalDataStruct);

	//Sends a Required Bool using a FBoolGlobalData struct. Calls SendRequiredBool under the hood.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DXCore|Requirement System")
	void SendRequiredBoolByStruct(FBoolGlobalData boolGlobalDataStruct);
//====

//====Save System

private:
	static const FString MainSaveGameName;
	
	//filled on initialization based on custom project settings for the plugin
	FString SaveGameTargetVersion;
	UPROPERTY()
	TSubclassOf<UDXCoreMainSaveGame> MainSaveGameClass;

	//Holder for currently loaded main save game
	UPROPERTY()
	UDXCoreMainSaveGame* _mainSaveGame;

	//Control Flags
	bool bDiskOperationInProgress = false;
	bool bSaveSystemIsCorrupt = false;
public:

	UFUNCTION(BlueprintPure, Category="DXCore|Save System")
	bool IsSaveSystemCorrupt() const { return bSaveSystemIsCorrupt; }

	UFUNCTION(BlueprintPure, Category="DXCore|Save System")
	static FString GetMainSaveGameName() { return MainSaveGameName; }

	/*Store any number of saved states, intended to store Actors SavedStates and used by checkpoints on activation.
	Must be unique name for all game.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DXCore|Save System")
	TMap<FName, FParameterKeeperData> ActorSavedStates;

	UPROPERTY(BlueprintAssignable, Category = "DXCore|Save System")
	FAroundSaveGameOperation OnPreSaveGame;
	UPROPERTY(BlueprintAssignable, Category = "DXCore|Save System")
	FAroundSaveGameOperation OnPostSaveGame;

	//Mainly used by GameSettings System
	UPROPERTY(BlueprintAssignable, Category = "DXCore|Save System")
	FAroundLoadGameOperation OnLoadGame;

	//Loads main save game from disk. It will override any memory instances.
	UFUNCTION(BlueprintCallable, Category = "DXCore|Save System", meta=(AutoCreateRefTerm = "PostLoadCallback"))
	void LoadMainSaveGame(const FMainSaveGameDiskOperation& PostLoadCallback);

	/*Returns the MainSaveGame. It should only return null when the save exists in disk but is not loaded.
	 * If the MainSaveGame is null and there is also no disk save to be loaded it will auto-create a new one
	 * assuming it has not been created yet. Use OutExecution for handling specific cases.*/
	UFUNCTION(BlueprintCallable, Category = "DXCore|Save System", meta=(ExpandEnumAsExecs="OutExecution"))
	UDXCoreMainSaveGame* GetMainSaveGame(EGetSave& OutExecution);

	//Saves current in-memory instance to disk. If in-memory instance is null it will fail.
	UFUNCTION(BlueprintCallable, Category = "DXCore|Save System", meta=(AutoCreateRefTerm = "PostSaveCallback"))
	void SaveMainSaveGame(const FMainSaveGameDiskOperation& PostSaveCallback);
	
	//Loads a new savegame object as currently loaded save game
	UFUNCTION(BlueprintCallable, Category = "DXCore|Save System")
	void UnloadMainSaveGame();

	/*Deletes the MainSaveGame for this user and clears memory-instance from memory.
	 * If disk file existed but fails to delete the memory-instance will not be cleared.
	 * If disk does not exist, only clears memory-instance from memory*/
	UFUNCTION(BlueprintCallable, Category = "DXCore|Save System")
	bool DeleteMainSaveGame();
	
	//====


	/* ---------- STEAMWORKS ---------- */
	UFUNCTION(BlueprintCallable, Category="DXCore|Steamworks")
	static UTexture2D* GetSteamAvatar(APlayerState* ps);

private:
#if PLATFORM_WINDOWS
	static class CSteamID SteamIDStringToCSteamID(FString s);
#endif
};
