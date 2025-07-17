using UnrealBuildTool;

public class CrimsonSkillTreeEditor : ModuleRules
{
    public CrimsonSkillTreeEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CrimsonSkillTree"
            ]
        );

        PrivateDependencyModuleNames.AddRange(
            [
                // Application & Input
                "ApplicationCore",
                "InputCore",

                // Blueprint, Graph, & Kismet Editing
                "BlueprintGraph",
                "GraphEditor",
                "Kismet",
                "KismetCompiler",
                "KismetWidgets",

                // Content Browser
                "ContentBrowser",
                "ContentBrowserData",

                // Core Engine & Utilities
                "CoreUObject",
                "DeveloperSettings",
                "Engine",
                "Projects", // Often related to loading/managing project-specific assets or settings
                "RenderCore",

                // Editor Framework & General UI
                "EditorFramework", // Core framework for editor functionality
                "EditorStyle",     // Styling specific to the editor
                "EditorWidgets",   // Common widgets used throughout the editor
                "MessageLog",      // For displaying messages, errors, warnings
                "ToolMenus",       // For extending editor menus and toolbars
                "UnrealEd",        // Main editor module
                "WorkspaceMenuStructure", // For managing editor tab/window layouts

                // Gameplay Systems
                "GameplayTags",

                // Property & Detail Customizations
                "DetailCustomizations", // For customizing details panels
                "PropertyEditor",     // Core module for property editing UI

                // Slate UI Framework
                "Slate",
                "SlateCore",
            ]
        );
    }
}