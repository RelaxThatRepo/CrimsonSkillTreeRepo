#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CrimsonSkillTree.generated.h"

class UCrimsonSkillTreeWidget_LineDrawingPolicyBase;
class UCrimsonSkillTreeWidget_Node;
class UCrimsonSkillTree_Edge;
class UCrimsonSkillTree_Node;
class UCrimsonSkillTree_VisualNode;
class UCrimsonSkillTreeManager;

USTRUCT(BlueprintType)
struct FCrimsonSkillTree_GraphConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCrimsonSkillTreeWidget_Node> DefaultNodeWidgetClass;

	/** @brief Maps a node's Gameplay Tag to a specific widget class, allowing for different visual styles for different node types. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, TSubclassOf<UCrimsonSkillTreeWidget_Node>> NodeTypeToWidgetClassMap;

	/** @brief This scales the location of the node, allowing the UEdGraph position to be "scaled" up or down. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Node")
	float NodePositionScalar = 1.f;

	/** @brief The line drawing policy object that defines the appearance and logic for drawing connection lines. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TObjectPtr<UCrimsonSkillTreeWidget_LineDrawingPolicyBase> LineDrawingPolicy;

	/** @brief The starting "Zoom" distance for the skill tree. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zoom")
	FVector2D StartingRenderScale = FVector2D(1.5f, 1.5f);

	/** @brief How fast you can zoom in/out. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zoom")
	float ZoomSpeedFactor = 0.07f;

	/** @brief The minimum allowed zoom level. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zoom")
	float MinZoomDistance = 0.75f;

	/** @brief The maximum allowed zoom level. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Zoom")
	float MaxZoomDistance = 1.75f;

	/** @brief Scalar offset along the X/Y-axis (0.0f to 2.0f, where 1.f is center, 0.f is the top, 2.f is the bottom.). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "View", meta = (ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0"))
	FVector2D ManualCenterOffset = FVector2D(1.f, 1.f);
	
};
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
	TArray<TObjectPtr<UCrimsonSkillTree_Node>>& GetAllNodes() { return AllNodes; }

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
	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree|Config")
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
	FString UserFacingRootNodeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Tree|Display")
	FCrimsonSkillTree_GraphConfig GraphConfig;
	// ~State
	// =============================================================================================================
	/** @brief The designated root node of the skill tree. All traversal typically starts from here. */
	UPROPERTY(Instanced, BlueprintReadOnly, Category = "Skill Tree|State")
	TObjectPtr<UCrimsonSkillTree_Node> RootNode;

	/** @brief A flat list of all nodes contained within this skill tree. */
	UPROPERTY(Instanced, BlueprintReadOnly, Category = "Skill Tree|State")
	TArray<TObjectPtr<UCrimsonSkillTree_Node>> AllNodes;

	/** @brief Array of all visual nodes used for annotation in this skill tree. */
	UPROPERTY(Instanced, BlueprintReadOnly, Category = "Skill Tree|State")
	TArray<TObjectPtr<UCrimsonSkillTree_VisualNode>> VisualNodes;

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
