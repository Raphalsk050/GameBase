
using UnrealBuildTool;

public class DXCoreEditor : ModuleRules
{
	public DXCoreEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "ApplicationCore" });

        PrivateDependencyModuleNames.Add("UnrealEd");
	}
}