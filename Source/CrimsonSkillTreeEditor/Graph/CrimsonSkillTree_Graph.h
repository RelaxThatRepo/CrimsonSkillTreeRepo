// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h" // Original include
#include "CrimsonSkillTree_Graph.generated.h"

class UCrimsonSkillTree;             
class UCrimsonSkillTree_Node;
class UCrimsonSkillTree_VisualNode;
class UCrimsonSkillTree_Edge;        
class UCrimsonSkillTree_GraphNode;
class UCrimsonSkillTree_GraphNode_Visual;
class UCrimsonSkillTree_GraphEdge;   
// class UCrimsonSkillTree_GraphNode_Root; // Not in original forward declarations, but used in UPROPERTY. TSubclassOf typically handles this.

/**
 * @class UCrimsonSkillTree_Graph
 * @brief Represents the editor graph for a Crimson Skill Tree asset (UCrimsonSkillTree).
 * This UEdGraph derivative manages the collection of graph nodes (UCrimsonSkillTree_GraphNode)
 * and graph edges (UCrimsonSkillTree_GraphEdge) that visually define the skill tree structure in the editor.
 * It is responsible for rebuilding the runtime UCrimsonSkillTree from this editor representation.
 */
UCLASS() // Preserving original UCLASS() without API macro
class CRIMSONSKILLTREEEDITOR_API UCrimsonSkillTree_Graph : public UEdGraph
{
	GENERATED_BODY()

public:
	//~ Properties
	/** @brief The class to use for the root node when creating a new graph or when a root node is needed. */
	UPROPERTY()
	TSubclassOf<class UCrimsonSkillTree_GraphNode_Root> RootNodeClass; // class keyword was in original

	/** * @brief Transient map from runtime UCrimsonSkillTree_Node instances to their corresponding
	 * editor UCrimsonSkillTree_GraphNode instances. Used during graph rebuilding and synchronization.
	 */
	UPROPERTY(Transient)
	TMap<TObjectPtr<UCrimsonSkillTree_Node>, TObjectPtr<UCrimsonSkillTree_GraphNode>> NodeMap;

	/** * @brief Transient map from runtime UCrimsonSkillTree_Edge instances to their corresponding
	 * editor UCrimsonSkillTree_GraphEdge instances. Used if edges have their own editor graph representation.
	 */
	UPROPERTY(Transient)
	TMap<TObjectPtr<UCrimsonSkillTree_Edge>, TObjectPtr<UCrimsonSkillTree_GraphEdge>> EdgeMap;

	/** @brief Transient map from runtime VisualNode to editor GraphNode_Visual */
	UPROPERTY(Transient)
	TMap<TObjectPtr<UCrimsonSkillTree_VisualNode>, TObjectPtr<UCrimsonSkillTree_GraphNode_Visual>> VisualNodeMap;
	//~ End Properties

	/**
	 * @brief Default constructor.
	 */
	UCrimsonSkillTree_Graph();

	//~ UObject Interface
	/**
	 * @brief Destructor.
	 */
	virtual ~UCrimsonSkillTree_Graph();

	/**
	 * @brief Marks the object as modified, optionally forcing the dirty flag.
	 * This version also marks the associated UCrimsonSkillTree asset and all its graph nodes as modified.
	 * @param bAlwaysMarkDirty If true, the object will always be marked dirty.
	 * @return True if the object was modified, false otherwise.
	 */
	virtual bool Modify(bool bAlwaysMarkDirty = true) override;

	/**
	 * @brief Called after an undo or redo operation has been performed on this graph.
	 * Typically used to refresh the graph's visual state.
	 */
	virtual void PostEditUndo() override;
	//~ End UObject Interface

	//~ Public Virtual Methods
	/**
	 * @brief Rebuilds the runtime UCrimsonSkillTree asset based on the current state of this editor graph.
	 * This involves iterating over graph nodes and edges to create or update their runtime counterparts.
	 */
	virtual void RebuildGraph();
	//~ End Public Virtual Methods

	//~ Public Methods
	/**
	 * @brief Gets the runtime UCrimsonSkillTree asset that this editor graph represents.
	 * This is typically the UObject outer of this UEdGraph instance.
	 * @return A pointer to the UCrimsonSkillTree asset.
	 */
	UCrimsonSkillTree* GetCrimsonSkillTree() const;
	//~ End Public Methods

protected:
	//~ Protected Methods
	/**
	 * @brief Clears the runtime UCrimsonSkillTree of its nodes and edges, and resets internal editor maps.
	 * This is usually a precursor to rebuilding the graph.
	 */
	void Clear();

	/**
	 * @brief Sorts the child and parent connections of nodes within the graph, typically based on their visual layout.
	 * @param RootNode The starting node for sorting, though the implementation may sort all nodes.
	 */
	void SortNodes(UCrimsonSkillTree_Node* RootNode);
	//~ End Protected Methods
};