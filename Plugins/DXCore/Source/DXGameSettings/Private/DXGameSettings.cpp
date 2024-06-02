#include "DXGameSettings.h"
#include "DXGameSettingsEditor.h"
#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FDXGameSettingsModule"

IMPLEMENT_MODULE(FDXGameSettings, DXGameSettings);

void FDXGameSettings::StartupModule()
{
	if (ISettingsModule* SettingModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingModule->RegisterSettings("Project", "DXCore", "GameSettings",
			LOCTEXT("RuntimeSettingsName", "Game Settings"),
			LOCTEXT("RuntimeSettingsDescription", "All user settings, that you want to put in your game."),
			GetMutableDefault<UDXGameSettingsEditor>()
		);
	}
}

void FDXGameSettings::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "DXCore", "GameSettings");
	}
}
