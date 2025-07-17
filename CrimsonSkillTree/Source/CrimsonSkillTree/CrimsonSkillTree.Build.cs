// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CrimsonSkillTree : ModuleRules
{
	public CrimsonSkillTree(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "GameplayAbilities", 
                "GameplayTags",      
                "ModelViewViewModel",
                "UMG"
            ]
        );

        PrivateDependencyModuleNames.AddRange(
            [
                // Core Engine & Utilities
                "CoreUObject",
                "DeveloperSettings",
                "Engine",
                "Projects",

                // Input
                "EnhancedInput",
                "InputCore",

                // Slate & UI
                "CommonUI",
                "Slate",
                "SlateCore",
                
            ]
        );

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(
                [
                    // Blueprint & Kismet (for custom Blueprint nodes)
                    "BlueprintGraph",
                    "KismetCompiler",

                    // Editor UI & Styling
                    "EditorStyle",
                    "EditorWidgets",
                    "PropertyEditor", // For custom details panel for skill tree assets
                    "UnrealEd",       // General editor functionality
                    "WorkspaceMenuStructure",
                ]
            );
        }

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
                // ... add any modules that your module loads dynamically here ...
            }
        );
	}
}
