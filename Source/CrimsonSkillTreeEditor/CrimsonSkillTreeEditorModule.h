#pragma once

#include "CoreMinimal.h"
#include "IAssetTools.h"               // For IAssetTools, EAssetTypeCategories::Type
#include "EdGraphUtilities.h"           // For FGraphPanelNodeFactory, FEdGraphUtilities
// #include "Factory/CrimsonSkillTree_NodeFactory.h" // Original include, assuming FGraphPanelNodeFactory_CrimsonSkillTree is defined elsewhere or forward declared implicitly by FGraphPanelNodeFactory usage.
// Forward declaring for now based on common practice if CrimsonSkillTree_NodeFactory.h isn't providing FGraphPanelNodeFactory.
// If FGraphPanelNodeFactory_CrimsonSkillTree is a concrete type defined in CrimsonSkillTree_NodeFactory.h, that include is necessary.
// For now, I'll assume FGraphPanelNodeFactory is the relevant type for the TSharedPtr.
class FGraphPanelNodeFactory_CrimsonSkillTree; // Forward declaration if the specific factory type is needed and defined in its own .h

#include "Modules/ModuleManager.h"

/** @brief Log category for the Crimson Skill Tree Editor module. */
DECLARE_LOG_CATEGORY_EXTERN(LogCrimsonSkillTreeEditor, All, All);

/**
 * @interface ICrimsonSkillTreeEditor
 * @brief Public interface for the CrimsonSkillTreeEditor module.
 * Provides a singleton-like access pattern to the module.
 */
class ICrimsonSkillTreeEditor : public IModuleInterface
{
public:
    /**
     * @brief Singleton-like access to this module's interface. This is just for convenience!
     * Beware of calling this during the shutdown phase, though. Your module might have been unloaded already.
     * @return Returns singleton instance, loading the module on demand if needed.
     */
    static ICrimsonSkillTreeEditor& Get()
    {
        return FModuleManager::LoadModuleChecked<ICrimsonSkillTreeEditor>("CrimsonSkillTreeEditor");
    }

    /**
     * @brief Checks to see if this module is loaded and ready. It is only valid to call Get() if IsAvailable() returns true.
     * @return True if the module is loaded and ready to use.
     */
    static bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("CrimsonSkillTreeEditor");
    }
};

/**
 * @class FCrimsonSkillTreeEditorModule
 * @brief The implementation of the Crimson Skill Tree Editor module.
 * This class handles the editor-specific functionalities for the Crimson Skill Tree plugin,
 * such as registering asset types, custom detail panels, graph editor factories, and editor styles.
 */
class FCrimsonSkillTreeEditorModule : public ICrimsonSkillTreeEditor
{
public:
    //~ IModuleInterface Interface
    /**
     * @brief Called when the module is starting up.
     * Registers asset type actions, graph factories, detail customizations, and editor styles.
     */
    virtual void StartupModule() override;

    /**
     * @brief Called when the module is shutting down.
     * Unregisters all components and cleans up resources used by the editor module.
     */
    virtual void ShutdownModule() override;
    //~ End IModuleInterface Interface

private:
    //~ Private Methods
    /**
     * @brief Helper function to register an asset type action with the AssetTools module
     * and add it to the list of created actions for later unregistration.
     * @param AssetTools Reference to the IAssetTools interface.
     * @param Action The asset type action to register.
     */
    void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

    /**
     * @brief Registers custom gameplay tags used by the Crimson Skill Tree plugin.
     * It adds the plugin's tag configuration directory to the UGameplayTagsManager search paths.
     */
    static void RegisterGameplayTags();

    /** Handler for when any UObject is saved in the editor. */
    void OnObjectSaved(UObject* SavedObject, FObjectPreSaveContext SaveContext);

    /** Handle for the post-save delegate. Used for unbinding during shutdown. */
    FDelegateHandle OnObjectSavedDelegateHandle;
    
    //~ End Private Methods

private:
    //~ Private Members
    /** @brief List of asset type actions created and registered by this module. Used for unregistration during shutdown. */
    TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;

    /** @brief Asset category bit used for registering the Crimson Skill Tree asset type. */
    EAssetTypeCategories::Type CrimsonSkillTreeAssetCategoryBit = EAssetTypeCategories::Type::Misc;

    /** @brief Shared pointer to the custom graph panel node factory for Crimson Skill Tree graphs. */
    TSharedPtr<FGraphPanelNodeFactory> GraphPanelNodeFactory_CrimsonSkillTree; // Using base FGraphPanelNodeFactory, specific type assumed to be FGraphPanelNodeFactory_CrimsonSkillTree
    //~ End Private Members
};