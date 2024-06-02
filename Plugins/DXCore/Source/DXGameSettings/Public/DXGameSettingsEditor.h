#pragma once

#include "Core/SettingObject.h"

#include "DXGameSettingsEditor.generated.h"

UCLASS(config = DXGameSettings, defaultconfig)
class DXGAMESETTINGS_API UDXGameSettingsEditor : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(config, EditAnywhere, Category = "DXGameSettings")
	TArray<TSubclassOf<USettingObject>> GameSettingObjectClasses;
};