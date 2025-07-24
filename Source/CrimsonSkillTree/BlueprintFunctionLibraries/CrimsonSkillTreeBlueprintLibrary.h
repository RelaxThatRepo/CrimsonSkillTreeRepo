#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "CrimsonSkillTreeBlueprintLibrary.generated.h"

class UCrimsonSkillTreeManager;

/**
 * @class UCrimsonSkillTreeBlueprintLibrary
 * @brief Provides globally accessible static functions for interacting with the Crimson Skill Tree system.
 */
UCLASS()
class CRIMSONSKILLTREE_API UCrimsonSkillTreeBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Admin Actions
	// =============================================================================================================
	/**
	 * @brief Globally accessible function to unlock a skill node and all of its descendants.
	 * @details This finds the Skill Tree Manager on the provided actor and calls the server-side function.
	 * @param WorldContextObject Any object in the world, used to get the game world.
	 * @param TargetActor The actor (e.g., Pawn, PlayerController) that has the UCrimsonSkillTreeManager component.
	 * @param SkillTreeTypeTag The tag identifying the skill tree containing the node.
	 * @param NodeName The display name of the starting node to unlock.
	 * @param bUnlockToMaxLevel If true, sets the node and its children to their maximum level. Otherwise, sets to level 1.
	 */
	UFUNCTION(BlueprintCallable, Category = "Crimson Skill Tree|Admin", meta = (WorldContext = "WorldContextObject"))
	static void ForceUnlockNodeAndDescendants(UObject* WorldContextObject, AActor* TargetActor, FGameplayTag SkillTreeTypeTag, const FText& NodeName, bool bUnlockToMaxLevel = true);

	/**
	 * @brief Globally accessible function to unlock a skill node and its descendants down to a specific depth.
	 * @details This finds the Skill Tree Manager on the provided actor and calls the server-side function.
	 * @param WorldContextObject Any object in the world, used to get the game world.
	 * @param TargetActor The actor (e.g., Pawn, PlayerController) that has the UCrimsonSkillTreeManager component.
	 * @param SkillTreeTypeTag The tag identifying the skill tree containing the node.
	 * @param NodeName The display name of the starting node to unlock.
	 * @param Depth How many levels of children to unlock. 0 means only the target node, -1 means all descendants.
	 * @param bUnlockToMaxLevel If true, sets the nodes to their maximum level. Otherwise, sets to level 1.
	 */
	UFUNCTION(BlueprintCallable, Category = "Crimson Skill Tree|Admin", meta = (WorldContext = "WorldContextObject"))
	static void ForceUnlockNodeAndDescendantsWithDepth(UObject* WorldContextObject, AActor* TargetActor, FGameplayTag SkillTreeTypeTag, const FText& NodeName, int32 Depth = 1, bool bUnlockToMaxLevel = true);

	// ~Utility
	// =============================================================================================================
	/**
	 * @brief Tries to find the CrimsonSkillTreeManager component from a given context object.
	 * @details It intelligently checks the object itself, its owner, or the local player's pawn/controller/state.
	 * This is the primary function to call from Blueprints to get a manager reference.
	 * @param WorldContextObject The object from which to start the search.
	 * @return The found UCrimsonSkillTreeManager, or nullptr if none is found in the chain.
	 */
	UFUNCTION(BlueprintPure, Category = "Skill Tree|Utilities", meta = (WorldContext = "WorldContextObject"))
	static UCrimsonSkillTreeManager* GetSkillTreeManager(const UObject* WorldContextObject);

private:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	/**
	 * @brief Helper function that performs the logical search for the manager component
	 * @details Starts from a specific actor and checks its controller and player state.
	 * @param InActor The actor to begin the search from.
	 * @return The found UCrimsonSkillTreeManager, or nullptr.
	 */
	static UCrimsonSkillTreeManager* FindManagerInActorChain(const AActor* InActor);
};
