#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeaturesSubsystem.h" // Required for FGameFeatureStateChangeContext
#include "CrimsonSkillTree/CrimsonSkillTreeManager.h" // Required for FCrimsonSkillTreeEntry
#include "GameFeatureAction_AddCrimsonSkillTree.generated.h"

struct FComponentRequestHandle;
class FDelegateHandle;
class UGameInstance;
class UObject;
struct FWorldContext;

/**
 * @struct FGameFeatureCrimsonSkillTreeEntry
 * @brief Defines a set of skill trees and associated settings to be added to a specific actor class via a Game Feature.
 */
USTRUCT()
struct FGameFeatureCrimsonSkillTreeEntry
{
	GENERATED_BODY()

	/** @brief The actor class to add the skill tree manager to. */
	UPROPERTY(EditAnywhere, Category = "Configuration")
	TSoftClassPtr<AActor> ActorClass;

	/** @brief The list of skill tree assets to configure on the manager. */
	UPROPERTY(EditAnywhere, Category = "Configuration")
	TArray<FCrimsonSkillTreeEntry> SkillTreesToAdd;

	//~ Save Game Settings
	// =============================================================================================================
	/** @brief If true, automatically call LoadAllSkillTreeStates() after skill trees are initialized. */
	UPROPERTY(EditAnywhere, Category = "Save Game")
	bool bLoadAllSkillTreesPostInitialize = true;

	/** @brief The name of the save game slot to use for skill tree data. */
	UPROPERTY(EditAnywhere, Category = "Save Game")
	FString SaveSlotName = "SkillTreeSaveSlot";

	/** @brief The user index for the save game slot. */
	UPROPERTY(EditAnywhere, Category = "Save Game")
	int32 SaveSlotUserIndex = 0;
};

/**
 * @class UGameFeatureAction_AddCrimsonSkillTree
 * @brief A Game Feature Action that adds a Crimson Skill Tree manager component and configures it on specified actors.
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Add Crimson Skill Tree"))
class UGameFeatureAction_AddCrimsonSkillTree final : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~UGameFeatureAction Interface
	// =============================================================================================================
	/**
	 * @brief Called when the game feature is being activated.
	 * @param Context The activation context.
	 */
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;

	/**
	 * @brief Called when the game feature is being deactivated.
	 * @param Context The deactivation context.
	 */
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	
	/** @brief Contains data for a specific FGameFeatureActivatingContext. */
	struct FPerContextData
	{
		/** @brief List of actors we have added the skill tree component to. */
		TMap<AActor*, TObjectPtr<UCrimsonSkillTreeManager>> ActiveExtensions;
		/** @brief Handles for component requests. */
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
	};
	
	/** @brief The list of actor classes and the skill trees to add to them. */
	UPROPERTY(EditAnywhere, Category = "Skill Tree", meta = (TitleProperty = "ActorClass", ShowOnlyInnerProperties))
	TArray<FGameFeatureCrimsonSkillTreeEntry> SkillTreeList;

private:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/
	/**
	 * @brief Applies the action to a given world.
	 * @param WorldContext The world context to apply the action to.
	 * @param ChangeContext The game feature state change context.
	 */
	void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext);

	/**
	 * @brief Callback for when a new game instance is started.
	 * @param GameInstance The new game instance.
	 * @param ChangeContext The game feature state change context.
	 */
	void HandleGameInstanceStart(UGameInstance* GameInstance, FGameFeatureStateChangeContext ChangeContext);

	/**
	 * @brief The callback fired by the Game Framework Component Manager when a relevant actor is ready.
	 * @param Actor The actor that was extended.
	 * @param EventName The name of the event (e.g., ExtensionAdded, ExtensionRemoved).
	 * @param EntryIndex The index into the SkillTreeList that this callback corresponds to.
	 * @param ChangeContext The game feature state change context.
	 */
	void HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex, FGameFeatureStateChangeContext ChangeContext);

	/**
	 * @brief Adds the skill tree manager and configured trees to a specific actor.
	 * @param Actor The actor to add the component to.
	 * @param SkillTreeEntry The configuration data for the skill trees.
	 * @param ActiveData The context-specific data for this activation.
	 */
	void AddSkillTreeToActor(AActor* Actor, const FGameFeatureCrimsonSkillTreeEntry& SkillTreeEntry, struct FPerContextData& ActiveData);

	/**
	 * @brief Removes the skill tree manager from a specific actor.
	 * @param Actor The actor to remove the component from.
	 * @param ActiveData The context-specific data for this activation.
	 */
	void RemoveSkillTreeFromActor(AActor* Actor, FPerContextData& ActiveData);

	/**
	 * @brief Resets the action's effects, removing components and clearing handles.
	 * @param ActiveData The context-specific data to reset.
	 */
	void Reset(FPerContextData& ActiveData);

private:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/

	/** @brief Map of context data, one entry for each activating context. */
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	/** @brief Map of handles to game instance start delegates. */
	TMap<FGameFeatureStateChangeContext, FDelegateHandle> GameInstanceStartHandles;
};
