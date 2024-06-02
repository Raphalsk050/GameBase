using UnrealBuildTool; 

public class DXCoreUI: ModuleRules 

{ 

    public DXCoreUI(ReadOnlyTargetRules Target) : base(Target) 
    {
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "UMG",
                "CommonUI",
                "CommonInput",
                "GameplayTags",
                "DXGameSettings"
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