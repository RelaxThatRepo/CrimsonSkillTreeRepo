#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CrimsonSkillTree.generated.h"

class UCrimsonSkillTree_Edge;
class UCrimsonSkillTree_Node;
class UCrimsonSkillTree_VisualNode;
class UCrimsonSkillTreeManager;

/**
 * @class UCrimsonSkillTree
 * @brief A data asset representing the structure of a skill tree, containing nodes and their relationships.
 * @details This object is used as a template and should be instanced at runtime by a UCrimsonSkillTreeManager.
 */
UCLASS(BlueprintType)
class CRIMSONSKILLTREE_API UCrimsonSkillTree : public UObject
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Construction
	// =============================================================================================================
	UCrimsonSkillTree();
	virtual ~UCrimsonSkillTree();

	// ~UObject Overrides
	// =============================================================================================================
	virtual void PostLoad() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// ~Core Functionality
	// =============================================================================================================
	/**
	 * @brief Resets all nodes in this tree to their default initial state.
	 * @details This is crucial before loading a saved game to ensure a clean slate.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
	void ResetTreeToDefaults();

	/**
	 * @brief Finds a node within this tree by its unique GUID.
	 * @param InNodeGUID The GUID of the node to find.
	 * @return The found UCrimsonSkillTree_Node, or nullptr if not found.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
	UCrimsonSkillTree_Node* FindNodeByGUID(const FGuid& InNodeGUID) const;

	/**
	 * @brief Gets a const reference to the flat list of all nodes contained within this skill tree.
	 * @return A const TArray reference of all nodes.
	 */
	const TArray<UCrimsonSkillTree_Node*>& GetAllNodes() const { return AllNodes; }

	/**
	 * @brief Gets a mutable reference to the flat list of all nodes contained within this skill tree.
	 * @return A TArray reference of all nodes.
	 */
	TArray<UCrimsonSkillTree_Node*>& GetAllNodes() { return AllNodes; }

	// ~Context
	// =============================================================================================================
	/**
	 * @brief Gets the manager component that owns this runtime instance of the skill tree.
	 * @return The owning UCrimsonSkillTreeManager, or nullptr if not set or invalid.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree")
	UCrimsonSkillTreeManager* GetOwningManager() const;

	// ~Diagnostics
	// =============================================================================================================
	/**
	 * @brief Prints a visual representation of the skill tree structure to the log and screen.
	 * @param bToConsole If true, output to the Unreal Engine console log.
	 * @param bToScreen If true, output as on-screen debug messages.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Diagnostics")
	void PrintTreeStructure(bool bToConsole = true, bool bToScreen = true) const;

#if WITH_EDITOR
	/**
	 * @brief [Editor] A helper function to print the number of baked nodes to the log.
	 */
	UFUNCTION(CallInEditor, Category = "Skill Tree|Diagnostics")
	void CheckBakedNodeCount();
#endif

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/

	// ~Delegates
	// =============================================================================================================
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillTreeInitialized);

	/** @brief Broadcasts after this specific skill tree has been initialized by the manager. */
	UPROPERTY(BlueprintAssignable, Category = "Skill Tree|Events")
	FOnSkillTreeInitialized OnSkillTreeInitialized;

	// ~Configuration
	// =============================================================================================================
	/** @brief A unique identifier for this skill tree asset. Essential for saving and loading. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Tree|Config")
	FGuid SkillTreeGUID;

	/** @brief The version of this skill tree. Increment this when making changes that should invalidate old save data. */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Config")
	int32 Version = 1;

	/** @brief The display name of the skill tree. */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Config")
	FString Name;

	/** @brief Gameplay tags associated with this skill tree for categorization and filtering. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Tree|Config")
	FGameplayTagContainer SkillTreeTags;

	// ~Editor Configuration
	// =============================================================================================================
	/** @brief The default class type for nodes created within this skill tree in the editor. */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Editor")
	TSubclassOf<UCrimsonSkillTree_Node> NodeType;

	/** @brief The default class type for edges created between nodes in this skill tree in the editor. */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Editor")
	TSubclassOf<UCrimsonSkillTree_Edge> EdgeType;

	/** @brief If true, edges are meaningful and processed in this tree. */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Editor")
	bool bEdgeEnabled;

	// ~Runtime Display
	// =============================================================================================================
	/** @brief If true, the root node will be explicitly displayed in widget representations. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Tree|Display")
	bool bDisplayRootNodeInWidget;

	/** @brief The display name of the node to use as the starting focus point when the real root node is hidden. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Tree|Display", meta = (EditCondition = "!bDisplayRootNodeInWidget"))
	FText UserFacingRootNodeName;

	// ~State
	// =============================================================================================================
	/** @brief The designated root node of the skill tree. All traversal typically starts from here. */
	UPROPERTY(Instanced, BlueprintReadOnly, Category = "Skill Tree|State")
	UCrimsonSkillTree_Node* RootNode;

	/** @brief A flat list of all nodes contained within this skill tree. */
	UPROPERTY(Instanced, BlueprintReadOnly, Category = "Skill Tree|State")
	TArray<UCrimsonSkillTree_Node*> AllNodes;

	/** @brief Array of all visual nodes used for annotation in this skill tree. */
	UPROPERTY(Instanced, BlueprintReadOnly, Category = "Skill Tree|State")
	TArray<UCrimsonSkillTree_VisualNode*> VisualNodes;

#if WITH_EDITORONLY_DATA
	/** @brief The graph representation of this skill tree used in the editor. */
	UPROPERTY()
	TObjectPtr<class UEdGraph> EdGraph;

	/** @brief If true, nodes in this skill tree can be renamed in the editor. */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Editor")
	bool bCanRenameNode;

	/** @brief If true, this skill tree can have cyclical relationships between nodes. */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Editor")
	bool bCanBeCyclical;
#endif

	// ~Transient State
	// =============================================================================================================
	/** @brief A weak pointer to the manager component that owns this runtime instance. */
	UPROPERTY(Transient)
	TWeakObjectPtr<UCrimsonSkillTreeManager> OwningManager;
};
