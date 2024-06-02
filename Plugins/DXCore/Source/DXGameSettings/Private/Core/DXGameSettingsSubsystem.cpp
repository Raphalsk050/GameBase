// Fill out your copyright notice in the Description page of Project Settings.
#include "Core/DXGameSettingsSubsystem.h"

#include "Core/DXCoreGISubsystem.h"
#include "DXGameSettingsEditor.h"
#include "GameFramework/GameUserSettings.h"

void UDXGameSettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency<UDXCoreGISubsystem>();

	// We need to first initialize all the settings with the default values, to avoid a dependency from the SaveSystem
	InitializeDefaultSettings();
	
	UDXCoreGISubsystem* coreGI = GetGameInstance()->GetSubsystem<UDXCoreGISubsystem>();
	check(coreGI);

	coreGI->OnUserSet.AddDynamic(this, &UDXGameSettingsSubsystem::ListenMainSaveGameInitializing);
}

void UDXGameSettingsSubsystem::InitializeDefaultSettings()
{
	const UDXGameSettingsEditor* gameSettings = GetDefault<UDXGameSettingsEditor>();
	check(gameSettings);

	// Create an instance for each setting object inside of GameSettingObjectClasses.
	for (TSubclassOf<USettingObject> SettingObjectClass : gameSettings->GameSettingObjectClasses)
	{
		USettingObject* SettingObject = NewObject<USettingObject>(USettingObject::StaticClass(), SettingObjectClass);
		_settingObjects.Add(SettingObject);
	}

	if (_settingObjects.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("There's are no Settings configured in this project to initialize."));
		return;
	}
	
	for (USettingObject* SettingObject : _settingObjects)
	{
		SettingObject->InitSetting(this);
		SettingObject->OnSettingChangeValue.AddDynamic(this, &UDXGameSettingsSubsystem::OnSettingValueChanged);
	}
}

void UDXGameSettingsSubsystem::ListenMainSaveGameInitializing()
{
	UDXCoreGISubsystem* coreGI = GetGameInstance()->GetSubsystem<UDXCoreGISubsystem>();
	check(coreGI);

	coreGI->OnLoadGame.AddDynamic(this, &UDXGameSettingsSubsystem::InitializeSavedSettings);
}

void UDXGameSettingsSubsystem::InitializeSavedSettings(bool SuccessLoadSavedData, UDXCoreMainSaveGame* CoreMainSaveGame)
{
	if (!SuccessLoadSavedData || !CoreMainSaveGame->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load saved data."));
		return;
	}
	
	if (_settingObjects.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("There's are no Settings configured in this project to load."));
		return;
	}

	// Get all saved game settings in the GameSettings slot.
	FParameterKeeperData gameSettingsOut;
	CoreMainSaveGame->GetGameSlotInfo("DXGameSettings", gameSettingsOut);

	TMap<FString, FString> allSavedSettings = gameSettingsOut.Strings;
	// Init all SettingObjects inside of _settingObjects with the saved value.
	for (TTuple<FString, FString> pair : gameSettingsOut.Strings)
	{
		if (USettingObject* settingObj = GetSettingObjectWithName(pair.Key))
		{
			settingObj->SetSavedValue(pair.Value);
			settingObj->ChangeSettingValue(pair.Value);
			allSavedSettings.Remove(pair.Key);
		}
	}

	// If there are still saved settings, but they are no longer being used,
	// the system considers that they are no longer useful and will delete them
	// if (!allSavedSettings.IsEmpty())
	// {
	// 	for (TTuple<FString, FString> pair : allSavedSettings)
	// 	{
	// 		gameSettingsOut.Strings.FindAndRemoveChecked(pair.Key);
	// 	}
	// }

	// CoreMainSaveGame->SetGameSlotInfo("DXGameSettings", gameSettingsOut);
	//
	// UDXCoreGISubsystem* coreGI = GetGameInstance()->GetSubsystem<UDXCoreGISubsystem>();
	// check(coreGI);
	//
	// FMainSaveGameDiskOperation outSave;
	// coreGI->SaveMainSaveGame(outSave);
	
	// Sync with Game User Settings
	for (USettingObject* settingObj : _settingObjects)
	{
		settingObj->SyncSavedWithGameUserSettings(UGameUserSettings::GetGameUserSettings());
	}
	UGameUserSettings::GetGameUserSettings()->ApplySettings(true);
}

void UDXGameSettingsSubsystem::OnSettingValueChanged(USettingObject* SettingChanged)
{
	// This conditional structure serves to administer the dirty settings.
	if (SettingChanged->IsDirty())
	{
		_dirtySettings.AddUnique(SettingChanged);
	}
	else
	{
		if(_dirtySettings.Contains(SettingChanged))
			_dirtySettings.Remove(SettingChanged);
	}

	// Delegate for objects relating to settings such as settings widgets
	OnSettingChange.Broadcast(SettingChanged->GetClass());
	
	// Delegate for the SettingWidget update the possible player actions in action bar.
	OnAnySettingsDirty.Broadcast(HasAnyDirtySetting());
}

void UDXGameSettingsSubsystem::ChangeSettingValue(TSubclassOf<USettingObject> Setting, FString NewValue)
{
	if (USettingObject* settingObject = GetSettingObjectOfClass(Setting))
	{
		settingObject->ChangeSettingValue(NewValue);
		//OnSettingValueChanged(settingObject);
	}
}

FString UDXGameSettingsSubsystem::GetSettingSavedValue(TSubclassOf<USettingObject> Setting)
{
	if (USettingObject* settingObject = GetSettingObjectOfClass(Setting))
	{
		return settingObject->GetSettingSavedValue();
	}
	return "";
}

FString UDXGameSettingsSubsystem::GetSettingCurrentValue(TSubclassOf<USettingObject> Setting)
{
	if (USettingObject* settingObject = GetSettingObjectOfClass(Setting))
	{
		return settingObject->GetSettingCurrentValue();
	}
	return "";
}

FOnSettingChangeValue* UDXGameSettingsSubsystem::GetSettingDelegate(TSubclassOf<USettingObject> Setting)
{
	if (USettingObject* settingObj = GetSettingObjectOfClass(Setting))
	{
		return &settingObj->OnSettingChangeValue;
	}

	return nullptr;
}

void UDXGameSettingsSubsystem::ApplySettings()
{
	UDXCoreGISubsystem* coreGI = GetGameInstance()->GetSubsystem<UDXCoreGISubsystem>();
	check(coreGI);

	EGetSave out;
	UDXCoreMainSaveGame* mainSaveGame = coreGI->GetMainSaveGame(out);
	if (!mainSaveGame)
		return;
	
	FParameterKeeperData settingsParameterKeeper;
	mainSaveGame->GetGameSlotInfo("DXGameSettings", settingsParameterKeeper);
	if(settingsParameterKeeper.IsEmpty())
	{
		mainSaveGame->CreateGameSlot("DXGameSettings", settingsParameterKeeper);
	}

	// Apply dirty settings into parameter keeper
	for (USettingObject* settingObject : _dirtySettings)
	{
		// The "Add" in context of TMaps serves to add a new key, or modify an existent one
		settingsParameterKeeper.Strings.Add(settingObject->SettingKey, settingObject->GetSettingCurrentValue());
	}
	
	// With the gameSettingsOut the system updates old setting values with newest
	mainSaveGame->SetGameSlotInfo("DXGameSettings", settingsParameterKeeper);

	// After SetGameSlotInfo, save to disk by calling SaveMainSaveGame
	FMainSaveGameDiskOperation outSave;
	coreGI->SaveMainSaveGame(outSave);

	// TODO: put this inside the save callback if successful
	UpdateSettingsSavedValues(settingsParameterKeeper);

	// // Sync with Game User Settings
	for (USettingObject* settingObj : _settingObjects)
	{
		settingObj->SyncSavedWithGameUserSettings(UGameUserSettings::GetGameUserSettings());
	}
	UGameUserSettings::GetGameUserSettings()->ApplySettings(true);
	
	_dirtySettings.Empty();

	OnAnySettingsDirty.Broadcast(false);
}

void UDXGameSettingsSubsystem::UpdateSettingsSavedValues(const FParameterKeeperData& AlreadySavedSettings)
{
	for (TTuple<FString, FString> pair : AlreadySavedSettings.Strings)
	{
		if (USettingObject* SettingObject = GetSettingObjectWithName(pair.Key))
		{
			SettingObject->SetSavedValue(pair.Value);
		}
	}
}

void UDXGameSettingsSubsystem::RevertSettings()
{
	// This array servers to avoid manipulation in the _dirtySettings during the iteration.
	TArray<USettingObject*> toRevertSetting;
	
	for (USettingObject* SettingObject : _dirtySettings)
	{
		toRevertSetting.Add(SettingObject);
	}

	for (USettingObject* SettingObject : toRevertSetting)
	{
		SettingObject->RevertSetting();
		
		// This always gonna true, but its good to check before remove to avoid crashes.
		if (_dirtySettings.Contains(SettingObject))
		{
			_dirtySettings.Remove(SettingObject);
		}
	}

	ensure(HasAnyDirtySetting() == false);
	OnAnySettingsDirty.Broadcast(HasAnyDirtySetting());
}

void UDXGameSettingsSubsystem::SetDefaultSettings()
{
	for (USettingObject* SettingObject : _settingObjects)
	{
		SettingObject->RevertSettingToDefault();
		//OnSettingValueChanged(SettingObject);
	}
	
	//OnAnySettingChange.Broadcast(HasAnyDirtySetting());

	//ApplySettings();
}

bool UDXGameSettingsSubsystem::HasAnyDirtySetting() const
{
	return _dirtySettings.Num() > 0;
}

USettingObject* UDXGameSettingsSubsystem::GetSettingObjectWithName(const FString& Name) const
{
	for (USettingObject* settingObject : _settingObjects)
	{
		if (settingObject->SettingKey == Name)
		{
			return settingObject;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Failed to get setting object with name %s"), *Name);
	return nullptr;
}

USettingObject* UDXGameSettingsSubsystem::GetSettingObjectOfClass(TSubclassOf<USettingObject> SettingObjectClass) const
{
	if (SettingObjectClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Setting class is null"));
		return nullptr;
	}
	
	for (USettingObject* settingObject : _settingObjects)
	{
		if (settingObject->GetClass() == SettingObjectClass || settingObject->IsA(SettingObjectClass))
		{
			return settingObject;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Failed to get setting object of class"));
	return nullptr;
}
