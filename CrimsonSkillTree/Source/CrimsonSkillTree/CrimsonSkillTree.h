// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CrimsonSkillTree.generated.h"

class UCrimsonSkillTree_Edge;
class UCrimsonSkillTree_Node;
class UCrimsonSkillTree_VisualNode;

/**
 * @class UCrimsonSkillTree
 * @brief A data asset representing the structure of a skill tree, containing nodes and their relationships.
 * This object is used as a template and should be instanced at runtime by a UCrimsonSkillTreeManager.
 */
UCLASS(Blueprintable)
class CRIMSONSKILLTREE_API UCrimsonSkillTree : public UObject
{
	GENERATED_BODY()

public:
	UCrimsonSkillTree();
	virtual ~UCrimsonSkillTree();
	
	//~ UObject Overrides
	virtual void PostLoad() override;
	//~ End UObject Overrides

	/**
 	* Creates a deep-copied, runtime-safe instance of this skill tree asset.
 	* This correctly instances all nodes and rebuilds their connections.
 	* @param Outer The object that should own the new runtime instance (typically the Skill Tree Manager component).
 	* @return A fully instanced, ready-to-use UCrimsonSkillTree object.
 	*/
	UFUNCTION(BlueprintCallable, Category = "CrimsonSkillTree")
	UCrimsonSkillTree* CreateRuntimeInstance(UObject* Outer);
	
	/**
	 * @brief Prints a visual representation of the skill tree structure to the log and screen.
	 * @param bToConsole If true, output to the Unreal Engine console log.
	 * @param bToScreen If true, output as on-screen debug messages.
	 */
	UFUNCTION(BlueprintCallable, Category = "CrimsonSkillTree")
	void PrintTreeStructure(bool bToConsole = true, bool bToScreen = true) const;
	
	/**
	 * @brief Resets all nodes in this tree to their default initial state.
	 * This is crucial before loading a saved game to ensure a clean slate.
	 */
	UFUNCTION(BlueprintCallable, Category = "CrimsonSkillTree")
	void ResetTreeToDefaults();

	/**
	 * @brief Gets a flat list of all nodes contained within this skill tree.
	 * @return A const reference to the TArray of all nodes.
	 */
	const TArray<TObjectPtr<UCrimsonSkillTree_Node>>& GetAllNodes() const { return AllNodes; }
	TArray<TObjectPtr<UCrimsonSkillTree_Node>>& GetAllNodes() { return AllNodes; }


	/**
	 * @brief Finds a node within this tree by its unique GUID.
	 * @param InNodeGUID The GUID of the node to find.
	 * @return The found UCrimsonSkillTree_Node, or nullptr if not found.
	 */
	UFUNCTION(BlueprintCallable, Category = "CrimsonSkillTree")
	UCrimsonSkillTree_Node* FindNodeByGUID(const FGuid& InNodeGUID) const;

#if WITH_EDITOR
	/** A helper button to check if the runtime node array has been populated. */
	UFUNCTION(CallInEditor, Category = "Skill Tree|Diagnostics")
	void CheckBakedNodeCount();
#endif


public:
	// A unique identifier for this skill tree asset. Essential for saving and loading.
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Skill Tree")
	FGuid SkillTreeGUID;

	// The version of this skill tree. Increment this when making changes that should invalidate old save data.
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree|Versioning")
	int32 Version = 1;
	
	// The display name of the skill tree.
	UPROPERTY(EditDefaultsOnly, Category = "Crimson|SkillTree")
	FString Name;

	// If true, the root node will be explicitly displayed in widget representations.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Runtime Display")
	bool bDisplayRootNodeInWidget;
	
	// The default class type for nodes created within this skill tree in the editor.
	UPROPERTY(EditDefaultsOnly, Category = "Crimson|SkillTree")
	TSubclassOf<UCrimsonSkillTree_Node> NodeType;

	// The default class type for edges created between nodes in this skill tree in the editor.
	UPROPERTY(EditDefaultsOnly, Category = "Crimson|SkillTree")
	TSubclassOf<UCrimsonSkillTree_Edge> EdgeType;

	// Gameplay tags associated with this skill tree for categorization and filtering.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crimson|SkillTree")
	FGameplayTagContainer SkillTreeTags;

	// The designated root node of the skill tree. All traversal typically starts from here.
	UPROPERTY(BlueprintReadOnly, Category = "Crimson|SkillTree")
	TObjectPtr<UCrimsonSkillTree_Node> RootNode;

	// A flat list of all nodes contained within this skill tree.
	UPROPERTY(BlueprintReadOnly, Category = "Crimson|SkillTree")
	TArray<TObjectPtr<UCrimsonSkillTree_Node>> AllNodes;

	/** @brief Array of all visual nodes used for annotation in this skill tree. */
	UPROPERTY(VisibleAnywhere, Instanced, Category = "Crimson|SkillTree")
	TArray<TObjectPtr<UCrimsonSkillTree_VisualNode>> VisualNodes;

	// If true, edges are meaningful and processed in this tree.
	UPROPERTY(EditDefaultsOnly, Category = "Crimson|SkillTree")
	bool bEdgeEnabled;

#if WITH_EDITORONLY_DATA
	// The graph representation of this skill tree used in the editor.
	UPROPERTY()
	TObjectPtr<class UEdGraph> EdGraph;

	// If true, nodes in this skill tree can be renamed in the editor.
	UPROPERTY(EditDefaultsOnly, Category = "GenericGraph_Editor")
	bool bCanRenameNode;

	// If true, this skill tree can have cyclical relationships between nodes.
	UPROPERTY(EditDefaultsOnly, Category = "GenericGraph_Editor")
	bool bCanBeCyclical;
#endif
};