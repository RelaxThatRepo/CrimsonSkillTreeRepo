// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTree.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Nodes/Cost/CrimsonSkillTree_NodeCost.h"
#include "Nodes/ICrimsonSkillTree_NodeAction.h"

#include "CrimsonSkillTreeManager.generated.h"

struct FCrimsonSkillTree_SaveGameData;
class UCrimsonSkillTree_SaveGame;
class UCrimsonSkillTree_Node;
class UCrimsonSkillTree;


/**
 * @enum ECrimsonSkillNodeActionType
 * @brief Defines the types of actions a user can request on a skill node.
 */
UENUM(BlueprintType)
enum class ECrimsonSkillNodeActionType : uint8
{
	Activate,
	Deactivate,
	IncrementLevel,
	DecrementLevel
};

/**
 * @struct FCrimsonSkillTreeEntry
 * @brief Defines a skill tree asset and its corresponding type tag for configuration.
 */
USTRUCT(BlueprintType)
struct FCrimsonSkillTreeEntry
{
	GENERATED_BODY()

	FCrimsonSkillTreeEntry() : SkillTreeAsset(nullptr) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree")
	FGameplayTag SkillTreeTypeTag; // e.g., "SkillTree.Type.Combat", "SkillTree.Type.Crafting"

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree")
	TObjectPtr<UCrimsonSkillTree> SkillTreeAsset; // The actual skill tree data asset
};

/**
 * @class UCrimsonSkillTreeManager
 * @brief Manages all skill tree instances for an actor, handling resource costs, state changes, and save/load operations.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRIMSONSKILLTREE_API UCrimsonSkillTreeManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UCrimsonSkillTreeManager();

	//~ UActorComponent Overrides
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End UActorComponent Overrides

	//~ Skill Tree Management
	/**
	 * @brief Gets a specific instanced skill tree by its type tag.
	 * @param SkillTreeTypeTag The tag identifying the skill tree.
	 * @return The instanced UCrimsonSkillTree, or nullptr if not found.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
	UCrimsonSkillTree* GetSkillTree(FGameplayTag SkillTreeTypeTag) const;

	/**
	 * @brief Initializes all configured skill trees, creating runtime instances.
	 * This is called automatically in BeginPlay.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
	void InitializeSkillTrees();
	//~ End Skill Tree Management

	//~ Node Actions
	/**
	 * @brief Server-authoritative function to perform an action on a skill node.
	 * @param TargetNode The node to perform the action on.
	 * @param ActionType The action to perform (Activate, Deactivate, etc.).
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestSkillNodeAction(UCrimsonSkillTree_Node* TargetNode, ECrimsonSkillNodeActionType ActionType);

	/**
 	* @brief Checks if a node can be assigned (activated for the first time).
 	* This checks prerequisites and resource costs.
 	* @param NodeToAssign The node to check.
 	* @return True if the node can be assigned.
 	*/
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Actions")
	bool CanAssignNode(UCrimsonSkillTree_Node* NodeToAssign);

	/**
 	* @brief Assigns a node, activating it and deducting costs.
 	* This should be called on the server.
 	* @param NodeToAssign The node to assign.
 	* @return True if the assignment was successful.
 	*/
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Actions")
	bool AssignNode(UCrimsonSkillTree_Node* NodeToAssign);

	/**
 	* @brief Checks if a node can be unassigned without invalidating other assigned nodes.
 	* @param NodeToUnassign The node being considered for unassignment.
 	* @param OutInvalidatedNodes (Output) A list of currently assigned nodes that would become invalid if NodeToUnassign is removed.
 	* @return True if the node can be unassigned without issues.
 	*/
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Actions")
	bool CanUnassignNode(const UCrimsonSkillTree_Node* NodeToUnassign, TArray<UCrimsonSkillTree_Node*>& OutInvalidatedNodes);

	/**
	 * @brief Unassigns a node, deactivating it and refunding costs.
	 * This should be called on the server.
	 * @param NodeToUnassign The node to unassign.
	 * @return True if unassignment was successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Actions")
	bool UnassignNode(UCrimsonSkillTree_Node* NodeToUnassign);

	/**
 	* @brief Checks if a node's level can be safely decremented without invalidating other nodes.
 	* @param NodeToDecrement The node to check.
 	* @param OutInvalidatedNodes (Output) A list of nodes that would be invalidated.
 	* @return True if the node can be safely decremented.
 	*/
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Actions")
	bool CanSafelyDecrementNodeLevel(UCrimsonSkillTree_Node* NodeToDecrement, TArray<UCrimsonSkillTree_Node*>& OutInvalidatedNodes);
	
	/**
	 * @brief Forcefully unassigns all active non-root nodes in a skill tree for a full respec.
	 * This bypasses normal dependency checks.
	 * @param SkillTreeTypeTag The tag identifying the skill tree to respec.
	 * @return True if the operation was successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Actions", meta = (DisplayName = "Force Respec Skill Tree (Unassign All Nodes)"))
	bool ForceUnassignAllNodesInTree(FGameplayTag SkillTreeTypeTag);

	/**
 	* @brief Server-authoritative function to unlock a node and all of its descendants, bypassing costs and prerequisites.
 	* @param TargetNode The starting node to unlock.
 	* @param bUnlockToMaxLevel If true, sets the node and its children to their maximum level. Otherwise, sets to level 1.
 	*/
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Skill Tree|Actions")
	void Server_ForceUnlockNodeAndDescendants(UCrimsonSkillTree_Node* TargetNode, bool bUnlockToMaxLevel = true);

	/**
	 * @brief Traverses every node in the specified skill trees and executes a provided action object on it.
	 * This is useful for performing batch UI operations or other actions on specific trees.
	 * @param TreesToTraverse An array of skill tree instances to traverse.
	 * @param NodeAction An instance of a class implementing ICrimsonSkillTree_NodeAction that defines the action to execute.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Utility")
	void TraverseTrees(const TArray<UCrimsonSkillTree*>& TreesToTraverse, TScriptInterface<ICrimsonSkillTree_NodeAction> NodeAction);
	//~ End Node Actions


	//~ Resource Management
	/**
	 * @brief Gets the total available amount of a specific resource for the owner.
	 * @param InCostDefinition The resource definition.
	 * @param OutTotalBudget (Output) The total amount of the resource the owner has.
	 * @return True if the budget was successfully retrieved.
	 */
	bool GetTotalBudgetForResource(const FNodeCostDefinition& InCostDefinition, int32& OutTotalBudget);

	/**
	 * @brief Gets the amount of a resource that has been allocated (spent) in this session.
	 * @param InCostDefinition The resource definition.
	 * @param OutAllocatedAmount (Output) The amount of the resource spent.
	 * @return True if the allocated amount was successfully retrieved.
	 */
	bool GetOverallAllocatedAmountForResource(const FNodeCostDefinition& InCostDefinition, int32& OutAllocatedAmount) const;

	/**
 	* @brief Gets the currently available (unspent) amount of a resource.
 	* @param InCostDefinition The resource definition.
 	* @param OutAvailableValue (Output) The available amount (Total - Spent).
 	* @return True if the value was successfully retrieved.
 	*/
	bool GetCurrentResourceValue(const FNodeCostDefinition& InCostDefinition, int32& OutAvailableValue);

	/**
	 * @brief Modifies the player's resource pool by a delta amount. This updates the manager's internal allocation tracking.
	 * @param InCostDefinition The resource definition.
	 * @param DeltaAmountToPlayerResourcePool The change to the player's resource pool (e.g., -5 to spend 5 points, +5 to refund 5 points).
	 * @return True if the modification was successful.
	 */
	bool ModifyResourceValue(const FNodeCostDefinition& InCostDefinition, int32 DeltaAmountToPlayerResourcePool);

	/**
	 * @brief Gathers all necessary data for displaying a resource in the UI.
	 * @param ForCostDefinition The resource definition.
	 * @param OutUsedAmount (Output) The amount of this resource currently spent.
	 * @param OutTotalAmount (Output) The total amount of this resource the owner has.
	 * @param OutResourceName (Output) The user-facing name of the resource.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|UI")
	void GetResourceUIData(const FNodeCostDefinition& ForCostDefinition, int32& OutUsedAmount, int32& OutTotalAmount, FText& OutResourceName);
	//~ End Resource Management


	//~ Save & Load
	/**
	 * @brief Saves the state of a single skill tree to the specified save slot.
	 * @param SkillTreeToSave The instanced skill tree to save.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|SaveGame")
	void SaveSkillTreeState(UCrimsonSkillTree* SkillTreeToSave);

	/**
	 * @brief Loads the state of a single skill tree from the specified save slot.
	 * @param SkillTreeToLoad The instanced skill tree to load state into.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|SaveGame")
	void LoadSkillTreeState(UCrimsonSkillTree* SkillTreeToLoad);

	/**
	 * @brief Loads the state for all configured skill trees from the save slot.
	 * This is the primary method for restoring player progress.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|SaveGame")
	void LoadAllSkillTreeStates();
	//~ End Save & Load

	//~ Owner & Context
	/**
	 * @brief This must be implemented if the "Owner" of the skill tree is not the AActor with the Component.
	 * For example, if the component is on a PlayerController but the owner is the Pawn.
	 * @return The AActor that owns the skill trees.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Tree")
	AActor* GetSkillTreeOwner();
	virtual AActor* GetSkillTreeOwner_Implementation();
	//~ End Owner & Context

	//~ Delegates
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPostInitializeSkillTrees);
	// Broadcasts after all skill trees have been initialized and their states have been loaded.
	UPROPERTY(BlueprintAssignable, Category="Skill Tree|Delegates")
	FPostInitializeSkillTrees PostInitializeSkillTrees;
	//~ End Delegates

protected:
	
	//~ Helper Functions
	/**
	 * @brief Finds an instanced skill tree by its unique GUID.
	 * @param InGuid The GUID of the tree to find.
	 * @return The instanced UCrimsonSkillTree, or nullptr if not found.
	 */
	UCrimsonSkillTree* GetSkillTreeByGUID(FGuid InGuid) const;

	/**
	 * @brief Modifies the internal tracking of allocated (spent) resources.
	 * @param InCostDefinition The resource definition.
	 * @param DeltaAllocation The change in allocation (e.g., +5 when spending 5 points, -5 when refunding 5 points).
	 * @return True if the modification was successful.
	 */
	bool ModifyOverallAllocationForResource(const FNodeCostDefinition& InCostDefinition, int32 DeltaAllocation);

	/**
	 * @brief Re-calculates all spent resource points based on the current state of all assigned nodes.
	 * Essential after loading a save game.
	 */
	void RebuildAllocatedResourceCache();

	/**
 	* @brief Calculates and refunds all costs based on an old, invalidated save data structure.
 	* @param SkillTree The current version of the skill tree asset (used to find node cost definitions).
 	* @param InvalidatedSaveData The out-of-date save data to process for refunds.
 	*/
	void RefundPointsFromInvalidatedSave(UCrimsonSkillTree* SkillTree, const FCrimsonSkillTree_SaveGameData& InvalidatedSaveData);


	/**
	 * @brief Loads or creates a save game object from the configured slot.
	 * @return A pointer to the loaded or newly created UCrimsonSkillTree_SaveGame object, or nullptr on failure.
	 */
	UCrimsonSkillTree_SaveGame* GetOrCreateSaveGameObject() const;

	/**
	 * @brief Finds the saved data for a specific tree within a save game object.
	 * @param SaveGameInstance The save game object to search within.
	 * @param TreeGuid The GUID of the tree to find data for.
	 * @return A pointer to the found save data, or nullptr if not found.
	 */
	const FCrimsonSkillTree_SaveGameData* FindTreeSaveData(const UCrimsonSkillTree_SaveGame* SaveGameInstance, FGuid TreeGuid) const;
	//~ End Helper Functions

public:
	// Array to configure skill tree templates in the editor. These will be instanced at runtime.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Trees|Configuration")
	TArray<FCrimsonSkillTreeEntry> ConfiguredSkillTrees;

	// A flat list of all currently assigned (active) nodes across all skill trees.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill Tree|Runtime")
	TArray<TObjectPtr<UCrimsonSkillTree_Node>> AssignedNodes;

	// Flag to indicate if a node action was initiated by a direct user request, used to control message broadcasting.
	UPROPERTY(Transient)
	bool bUserRequestedAction = false;

	// If true, automatically call LoadAllSkillTreeStates() after skill trees are initialized in BeginPlay.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Trees|Save Game")
	bool bLoadAllSkillTreesPostInitialize = true;

	// The name of the save game slot to use for skill tree data.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Trees|Save Game")
	FString SaveSlotName = "SkillTreeSaveSlot";

	// The user index for the save game slot.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Trees|Save Game")
	int32 SaveSlotUserIndex = 0;

protected:
	// Runtime map for quick access to instanced skill trees, populated from ConfiguredSkillTrees.
	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UCrimsonSkillTree>> ActiveSkillTreeInstances;
	
	// Caches the total points spent from actor integer properties during this session.
	UPROPERTY(Transient)
	TMap<FName, int32> SessionAllocatedActorPropertyPoints;

	// Caches the total points spent from gameplay attributes during this session.
	UPROPERTY(Transient)
	TMap<FGameplayAttribute, int32> SessionAllocatedGameplayAttributePoints;


};