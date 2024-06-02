#include "Modules/ModuleManager.h"

class FDXCoreUI : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE(FDXCoreUI, DXCoreUI);

void FDXCoreUI::StartupModule()
{
}

void FDXCoreUI::ShutdownModule()
{
}

