#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrimsonSkillTree.h"
#include "GameplayTagContainer.h"
#include "Nodes/Cost/CrimsonSkillTree_NodeCost.h"
#include "Nodes/CrimsonSkillTree_Node.h"
#include "Nodes/ICrimsonSkillTree_NodeAction.h"
#include "CrimsonSkillTreeManager.generated.h"

struct FCrimsonSkillTree_SaveGameData;
class UCrimsonSkillTree_SaveGame;

/**
 * @struct FReplicatedResourceAllocation
 * @brief Holds the amount of a specific resource that has been spent and replicated.
 */
USTRUCT()
struct FReplicatedResourceAllocation
{
	GENERATED_BODY()

	UPROPERTY()
	FNodeCostDefinition CostDefinition;

	UPROPERTY()
	int32 AllocatedAmount = 0;

	bool operator==(const FNodeCostDefinition& Other) const { return CostDefinition == Other; }
};

/**
 * @struct FReplicatedNodeState
 * @brief Holds the minimal replicated state for a single skill node.
 */
USTRUCT()
struct FReplicatedNodeState
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid NodeGUID;

	UPROPERTY()
	int32 CurrentLevel = 0;

	UPROPERTY()
	ENodeState NodeState = ENodeState::UnSet;

	bool operator==(const FGuid& OtherGuid) const { return NodeGUID == OtherGuid; }
};

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
	FGameplayTag SkillTreeTypeTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Tree")
	TObjectPtr<UCrimsonSkillTree> SkillTreeAsset;
};

/**
 * @class UCrimsonSkillTreeManager
 * @brief Manages all skill tree instances for an actor, handling state, resources, and save/load operations.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRIMSONSKILLTREE_API UCrimsonSkillTreeManager : public UActorComponent
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Construction
	// =============================================================================================================
	UCrimsonSkillTreeManager();

	// ~UActorComponent Overrides
	// =============================================================================================================
	/**
	 * @brief Called when the game starts for this component.
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Defines the properties that should be replicated.
	 * @param OutLifetimeProps Array to which replicated properties are added.
	 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
	 * @brief Determines if this component class can be replicated.
	 * @return True if the component can be replicated, false otherwise.
	 */
	virtual bool GetComponentClassCanReplicate() const override { return true; };

	// ~Skill Tree Management
	// =============================================================================================================
	/**
	 * @brief Initializes all configured skill trees for this manager.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Management")
	void InitializeSkillTrees();

	/**
	 * @brief Retrieves a skill tree instance by its type tag.
	 * @param SkillTreeTypeTag The tag identifying the skill tree type.
	 * @return The found skill tree instance, or nullptr if not found.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Management")
	UCrimsonSkillTree* GetSkillTree(FGameplayTag SkillTreeTypeTag) const;

	/**
	 * @brief Finds and sets the root node for a given skill tree.
	 * @param InSkillTree The skill tree to process.
	 * @return The found root node, or nullptr if no unique root is found.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Management")
	UCrimsonSkillTree_Node* FindAndSetRootNode(UCrimsonSkillTree* InSkillTree);

	// ~Node Actions
	// =============================================================================================================
	/**
	 * @brief Checks if a node can be assigned (activated).
	 * @param NodeToAssign The node to check.
	 * @return True if the node can be assigned, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Actions")
	bool CanAssignNode(UCrimsonSkillTree_Node* NodeToAssign);

	/**
	 * @brief Assigns (activates) a skill node.
	 * @param NodeToAssign The node to assign.
	 * @return True if the assignment was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Actions")
	bool AssignNode(UCrimsonSkillTree_Node* NodeToAssign);

	/**
	 * @brief Unassigns (deactivates) a skill node.
	 * @param NodeToUnassign The node to unassign.
	 * @return True if the unassignment was successful, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Actions")
	bool UnassignNode(UCrimsonSkillTree_Node* NodeToUnassign);

	/**
	 * @brief Checks if a node's level can be safely decremented without invalidating other nodes.
	 * @param NodeToDecrement The node to check.
	 * @param OutInvalidatedNodes An array that will be populated with any nodes that would become invalid.
	 * @return True if the decrement is safe, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Actions")
	bool CanSafelyDecrementNodeLevel(UCrimsonSkillTree_Node* NodeToDecrement, TArray<UCrimsonSkillTree_Node*>& OutInvalidatedNodes);

	// ~Resource Management
	// =============================================================================================================
	/**
	 * @brief Gets the total budget for a specific resource type.
	 * @param InCostDefinition The definition of the resource.
	 * @param OutTotalBudget The total available amount of the resource.
	 * @return True if the budget was successfully retrieved, false otherwise.
	 */
	bool GetTotalBudgetForResource(const FNodeCostDefinition& InCostDefinition, int32& OutTotalBudget);

	/**
	 * @brief Gets the total amount of a resource that has been allocated (spent).
	 * @param InCostDefinition The definition of the resource.
	 * @param OutAllocatedAmount The amount of the resource that has been spent.
	 * @return True if the allocated amount was successfully retrieved, false otherwise.
	 */
	bool GetOverallAllocatedAmountForResource(const FNodeCostDefinition& InCostDefinition, int32& OutAllocatedAmount) const;

	/**
	 * @brief Gets the current available value of a resource (Total - Allocated).
	 * @param InCostDefinition The definition of the resource.
	 * @param OutAvailableValue The currently available amount of the resource.
	 * @return True if the value was successfully calculated, false otherwise.
	 */
	bool GetCurrentResourceValue(const FNodeCostDefinition& InCostDefinition, int32& OutAvailableValue);

	/**
	 * @brief Modifies the player's resource pool by a delta amount. This is a wrapper for modifying the allocation.
	 * @param InCostDefinition The definition of the resource.
	 * @param DeltaAmountToPlayerResourcePool The amount to add (positive) or remove (negative) from the player's pool.
	 * @return True if the modification was successful, false otherwise.
	 */
	bool ModifyResourceValue(const FNodeCostDefinition& InCostDefinition, int32 DeltaAmountToPlayerResourcePool);

	/**
	 * @brief Retrieves UI-relevant data for a resource, such as used amount, total amount, and name.
	 * @param ForCostDefinition The definition of the resource.
	 * @param OutUsedAmount The amount of the resource currently used.
	 * @param OutTotalAmount The total amount of the resource available.
	 * @param OutResourceName The display name of the resource.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|UI")
	void GetResourceUIData(const FNodeCostDefinition& ForCostDefinition, int32& OutUsedAmount, int32& OutTotalAmount, FText& OutResourceName);

	// ~Save & Load
	// =============================================================================================================
	/**
	 * @brief Saves the state of a single skill tree to the save game slot.
	 * @param SkillTreeToSave The skill tree instance to save.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|SaveGame")
	void SaveSkillTreeState(UCrimsonSkillTree* SkillTreeToSave);

	/**
	 * @brief Loads the state of a single skill tree from the save game slot.
	 * @param SkillTreeToLoad The skill tree instance to load state into.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|SaveGame")
	void LoadSkillTreeState(UCrimsonSkillTree* SkillTreeToLoad);

	/**
	 * @brief Loads the state for all configured skill trees from the save game slot.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|SaveGame")
	void LoadAllSkillTreeStates();

	// ~Owner & Context
	// =============================================================================================================
	/**
	 * @brief Gets the owning actor of this skill tree manager.
	 * @return The owning actor.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Tree|Context")
	AActor* GetSkillTreeOwner();
	virtual AActor* GetSkillTreeOwner_Implementation();

	// ~Utility
	// =============================================================================================================
	/**
	 * @brief Traverses a list of skill trees and executes an action on each node.
	 * @param TreesToTraverse The array of skill trees to traverse.
	 * @param NodeAction The action to execute on each node.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Utility")
	void TraverseTrees(const TArray<UCrimsonSkillTree*>& TreesToTraverse, TScriptInterface<ICrimsonSkillTree_NodeAction> NodeAction);

	// ~Server RPCs
	// =============================================================================================================
	/**
	 * @brief [Server] Requests a specific action (e.g., activate, level up) to be performed on a skill node.
	 * @param TargetNodeGuid The GUID of the node to perform the action on.
	 * @param ActionType The type of action to perform.
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestSkillNodeAction(const FGuid& TargetNodeGuid, ECrimsonSkillNodeActionType ActionType);

	/**
	 * @brief [Server] Forces all nodes in a specific skill tree to be unassigned (a "respec").
	 * @param SkillTreeTypeTag The tag identifying the skill tree to respec.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Skill Tree|Admin", meta = (DisplayName = "Force Respec Skill Tree (Unassign All Nodes)"))
	void Server_ForceUnassignAllNodesInTree(FGameplayTag SkillTreeTypeTag);

	/**
	 * @brief [Server] Forces a single node to be unlocked.
	 * @param SkillTreeTypeTag The tag of the tree containing the node.
	 * @param NodeName The display name of the node to unlock.
	 * @param bUnlockToMaxLevel If true, unlocks the node to its maximum level.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Skill Tree|Admin")
	void Server_ForceUnlockNode(const FGameplayTag SkillTreeTypeTag, const FText& NodeName, const bool bUnlockToMaxLevel = true);

	/**
	 * @brief [Server] Forces a node and all of its descendants to be unlocked.
	 * @param SkillTreeTypeTag The tag of the tree containing the node.
	 * @param NodeName The display name of the starting node.
	 * @param bUnlockToMaxLevel If true, unlocks all affected nodes to their maximum level.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Skill Tree|Admin")
	void Server_ForceUnlockNodeAndDescendants(const FGameplayTag SkillTreeTypeTag, const FText& NodeName, const bool bUnlockToMaxLevel = true);

	/**
	 * @brief [Server] Forces a node and its descendants up to a specific depth to be unlocked.
	 * @param SkillTreeTypeTag The tag of the tree containing the node.
	 * @param NodeName The display name of the starting node.
	 * @param Depth The maximum depth of descendants to unlock (-1 for infinite).
	 * @param bUnlockToMaxLevel If true, unlocks all affected nodes to their maximum level.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Skill Tree|Admin")
	void Server_ForceUnlockNodeAndDescendantsWithDepth(const FGameplayTag SkillTreeTypeTag, const FText& NodeName, const int32 Depth = 1, const bool bUnlockToMaxLevel = true);

	// ~Internal State Management
	// =============================================================================================================
	/**
	 * @brief [Server] Adds or updates the replicated state for a given node.
	 * @param Node The node whose state needs to be updated.
	 */
	void Server_UpdateReplicatedNodeState(UCrimsonSkillTree_Node* Node);

	/**
	 * @brief [Server] Removes the replicated state for a given node.
	 * @param Node The node whose state needs to be removed.
	 */
	void Server_RemoveReplicatedNodeState(UCrimsonSkillTree_Node* Node);

	/**
	 * @brief Relays a node failure message from the server to the owning client via an RPC.
	 * @param Message The UI message payload to be sent.
	 */
	void SendFailureMessageToClient(const FCrimsonSkillTree_NodeUIMessage& Message);

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/

	// ~Delegates
	// =============================================================================================================
	/**
	 * @brief Broadcasts after all skill trees have been initialized.
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPostInitializeSkillTrees);
	UPROPERTY(BlueprintAssignable, Category = "Skill Tree|Delegates")
	FPostInitializeSkillTrees PostInitializeSkillTrees;

	/**
	 * @brief Broadcasts when the replicated state of any skill tree node has been updated.
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillTreeStateUpdated);
	UPROPERTY(BlueprintAssignable, Category = "Skill Tree|Delegates")
	FOnSkillTreeStateUpdated OnSkillTreeStateUpdated;

	/**
	 * @brief Broadcasts when a resource cost has been modified (e.g., a node is assigned or unassigned).
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillTreeModifyResourceCost);
	UPROPERTY(BlueprintAssignable, Category = "Skill Tree|Delegates")
	FOnSkillTreeModifyResourceCost OnSkillTreeModifyResourceCost;

	// ~Configuration
	// =============================================================================================================
	/**
	 * @brief The list of skill tree assets to be managed by this component.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Trees|Configuration")
	TArray<FCrimsonSkillTreeEntry> ConfiguredSkillTrees;

	// ~Save Game
	// =============================================================================================================
	/**
	 * @brief If true, automatically loads all skill tree states after initialization.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Trees|Save Game")
	bool bLoadAllSkillTreesPostInitialize = true;

	/**
	 * @brief The name of the save game slot to use.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Trees|Save Game")
	FString SaveSlotName = "SkillTreeSaveSlot";

	/**
	 * @brief The user index for the save game slot.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Trees|Save Game")
	int32 SaveSlotUserIndex = 0;

	// ~State
	// =============================================================================================================
	/**
	 * @brief Transient flag to indicate if a node action was initiated by a direct user request.
	 */
	UPROPERTY(Transient)
	bool bUserRequestedAction = false;

protected:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Initialization
	// =============================================================================================================
	/**
	 * @brief [Server] Creates runtime instances of all configured skill trees on the server.
	 */
	UFUNCTION(Server, Reliable)
	void Server_InitializeSkillTrees();

	/**
	 * @brief [Client] Creates local runtime instances of all configured skill trees on the client.
	 */
	void Client_InitializeSkillTreeInstances();

	/**
	 * @brief Creates a deep-copy runtime instance of a skill tree asset.
	 * @param InSkillTree The skill tree asset to instance.
	 * @return A new runtime instance of the skill tree.
	 */
	UCrimsonSkillTree* CreateSkillTreeRuntimeInstance(const UCrimsonSkillTree* InSkillTree);

	// ~Replication
	// =============================================================================================================
	/**
	 * @brief [Client] Called when the ReplicatedNodeStates array is updated.
	 */
	UFUNCTION()
	void OnRep_ReplicatedNodeStates();

	/**
	 * @brief Called when the ReplicatedAllocatedResources array is updated.
	 */
	UFUNCTION()
	void OnRep_AllocatedResourcesChanged();

	// ~Messaging
	// =============================================================================================================
	/**
	 * @brief [Client] Receives a failure message and broadcasts it to the local UI message system.
	 * @param Message The UI message payload received from the server.
	 */
	UFUNCTION(Client, Unreliable)
	void Client_DisplayNodeFailureMessage(const FCrimsonSkillTree_NodeUIMessage& Message);

	// ~Helpers
	// =============================================================================================================
	/**
	 * @brief Finds a skill tree instance by its unique GUID.
	 * @param InGuid The GUID of the skill tree to find.
	 * @return The found skill tree instance, or nullptr.
	 */
	UCrimsonSkillTree* GetSkillTreeByGUID(FGuid InGuid) const;

	/**
	 * @brief Finds a node within any managed skill tree by its unique GUID.
	 * @param NodeGuid The GUID of the node to find.
	 * @return The found node, or nullptr.
	 */
	UCrimsonSkillTree_Node* FindNodeByGuid(const FGuid& NodeGuid) const;

	/**
	 * @brief Finds a node within a specific skill tree by its display name.
	 * @param SkillTree The skill tree to search within.
	 * @param NodeName The display name of the node to find.
	 * @return The found node, or nullptr.
	 */
	UCrimsonSkillTree_Node* FindNodeInTreeByName(const UCrimsonSkillTree* SkillTree, FText NodeName) const;

	/**
	 * @brief [Server] Modifies the overall allocated amount for a resource.
	 * @param InCostDefinition The definition of the resource.
	 * @param DeltaAllocation The amount to add or remove from the allocation.
	 * @return True if the modification was successful.
	 */
	bool ModifyOverallAllocationForResource(const FNodeCostDefinition& InCostDefinition, int32 DeltaAllocation);

	/**
	 * @brief [Server] Recalculates and rebuilds the cache of all allocated resources.
	 */
	void RebuildAllocatedResourceCache();

	/**
	 * @brief [Server] Refunds resource points from a save file that is from an older, incompatible version of a skill tree.
	 * @param SkillTree The new skill tree asset.
	 * @param InvalidatedSaveData The save data from the old version.
	 */
	void RefundPointsFromInvalidatedSave(UCrimsonSkillTree* SkillTree, const FCrimsonSkillTree_SaveGameData& InvalidatedSaveData);

	/**
	 * @brief Gets the existing save game object or creates a new one if it doesn't exist.
	 * @return The save game object instance.
	 */
	UCrimsonSkillTree_SaveGame* GetOrCreateSaveGameObject() const;

	/**
	 * @brief Finds the save data for a specific skill tree within a save game object.
	 * @param SaveGameInstance The save game object to search in.
	 * @param TreeGuid The GUID of the skill tree to find data for.
	 * @return A pointer to the found save data, or nullptr.
	 */
	const FCrimsonSkillTree_SaveGameData* FindTreeSaveData(const UCrimsonSkillTree_SaveGame* SaveGameInstance, FGuid TreeGuid) const;

protected:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/

	// ~State
	// =============================================================================================================
	/**
	 * @brief Non-replicated array of active skill tree instances, created locally on both server and client.
	 */
	UPROPERTY(Transient)
	TArray<UCrimsonSkillTree*> ActiveSkillTreeInstances;

	// ~Replicated State
	// =============================================================================================================
	/**
	 * @brief Replicated array holding the minimal state data for all assigned nodes.
	 */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_ReplicatedNodeStates)
	TArray<FReplicatedNodeState> ReplicatedNodeStates;

	/**
	 * @brief Replicated array holding the total amount of each resource type that has been spent.
	 */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_AllocatedResourcesChanged)
	TArray<FReplicatedResourceAllocation> ReplicatedAllocatedResources;
};
