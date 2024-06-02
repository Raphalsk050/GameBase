using UnrealBuildTool; 

public class DXTweakingManager: ModuleRules 

{ 

    public DXTweakingManager(ReadOnlyTargetRules Target) : base(Target) 
    {
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "UMG",
                "CommonUI",
                "HTTP",
                "Json"
            }
            );

        PrivateDependencyModuleNames.AddRange(
            new string[] 
            {
                "Core",
                "CoreUObject",
                "Engine",
                "DXCore",
                "Slate",
                "SlateCore",
                "HTTP", 
                "Json", 
               
            }
            );
    } 

}