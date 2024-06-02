// Copyright Epic Games, Inc. All Rights Reserved.

#include "DXCore.h"

#include "Developer/Settings/Public/ISettingsModule.h"
#include "Developer/Settings/Public/ISettingsSection.h"
#include "Developer/Settings/Public/ISettingsContainer.h"
#include "Core/DXCoreSettings.h"

#define LOCTEXT_NAMESPACE "FDXCoreModule"

DEFINE_LOG_CATEGORY(DXCore);

void FDXCoreModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	RegisterSettings();
	
}

void FDXCoreModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (UObjectInitialized())
	{
		UnregisterSettings();
	}
}

bool FDXCoreModule::SupportsDynamicReloading()
{
	return true;
}

bool FDXCoreModule::HandleSettingsSaved()
{
	UDXCoreSettings* Settings = GetMutableDefault<UDXCoreSettings>();
	bool ResaveSettings = false;

	// You can put any validation code in here and resave the settings in case an invalid
	// value has been entered
	ResaveSettings = true;

	if (ResaveSettings)
	{
		Settings->SaveConfig();
	}

	return true;
}

void FDXCoreModule::RegisterSettings()
{
	// Registering some settings is just a matter of exposing the default UObject of
	// your desired class, feel free to add here all those settings you want to expose
	// to your LDs or artists.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Create the new category
		ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

		SettingsContainer->DescribeCategory("DXCore",
            LOCTEXT("RuntimeWDCategoryName", "DXCore"),
            LOCTEXT("RuntimeWDCategoryDescription", "Game configuration for the DXCore game module"));

		// Register the settings
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "DXCore", "General",
            LOCTEXT("RuntimeGeneralSettingsName", "General"),
            LOCTEXT("RuntimeGeneralSettingsDescription", "Base configuration for our game module"),
            GetMutableDefault<UDXCoreSettings>()
            );

		// Register the save handler to your settings, you might want to use it to
		// validate those or just act to settings changes.
		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FDXCoreModule::HandleSettingsSaved);
		}
	}
}

void FDXCoreModule::UnregisterSettings()
{
	// Ensure to unregister all of your registered settings here, hot-reload would
	// otherwise yield unexpected results.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "DXCore", "General");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDXCoreModule, DXCore)