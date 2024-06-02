#pragma once

#include "CoreMinimal.h"
#include "Core/SettingObject.h"
#include "UObject/Object.h"
#include "Modules/ModuleInterface.h"

class FDXGameSettings : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
