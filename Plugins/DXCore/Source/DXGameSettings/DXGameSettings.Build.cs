using UnrealBuildTool; 

public class DXGameSettings : ModuleRules 
{ 
    public DXGameSettings(ReadOnlyTargetRules Target) : base(Target) 
    {
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "UMG",
                "GameplayTags",
                "DXCore"
            }
            );

        PrivateDependencyModuleNames.AddRange(
            new string[] 
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore"
            }
            );
    }
}