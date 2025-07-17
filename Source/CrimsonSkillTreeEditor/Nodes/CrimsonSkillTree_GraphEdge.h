// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTreeEditor/Nodes/CrimsonSkillTree_GraphNode.h" // Original include
#include "CrimsonSkillTree_GraphEdge.generated.h"

class UCrimsonSkillTree_Edge; // Original forward declaration
/**
 * @class UCrimsonSkillTree_GraphEdge
 * @brief Represents an edge in the editor graph for a Crimson Skill Tree.
 * This UEdGraphNode derivative visually connects two UCrimsonSkillTree_GraphNode instances
 * and typically corresponds to a runtime UCrimsonSkillTree_Edge.
 */
UCLASS(MINimalAPI) // Preserving original MINimalAPI
class UCrimsonSkillTree_GraphEdge : public UEdGraphNode
{
	GENERATED_BODY()

public:
	//~ Properties
	/** @brief Pointer to the UEdGraph that this edge belongs to. Often the main skill tree graph. */
	UPROPERTY()
	TObjectPtr<class UEdGraph> SkillTree; // class keyword was in original for UEdGraph

	/** @brief Pointer to the runtime UCrimsonSkillTree_Edge object that this graph edge represents. */
	UPROPERTY()
	TObjectPtr<UCrimsonSkillTree_Edge> SkillTree_Edge;
	//~ End Properties

	/**
	 * @brief Default constructor.
	 */
	UCrimsonSkillTree_GraphEdge();

	//~ UEdGraphNode Interface
	/**
	 * @brief Allocates the default input and output pins for this edge node.
	 * Edges typically have hidden pins representing their connections.
	 */
	virtual void AllocateDefaultPins() override;

	/**
	 * @brief Gets the title of this edge node for display in the graph editor.
	 * @param TitleType The type of title requested (e.g., full, editable).
	 * @return The title text. Can be empty if the underlying runtime edge has no title.
	 */
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	/**
	 * @brief Called when the connection list for a pin on this node changes.
	 * If an edge node loses its connections, it may destroy itself.
	 * @param Pin The pin whose connection list has changed.
	 */
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	/**
	 * @brief Prepares this graph edge for copying (e.g., during copy-paste operations in the editor).
	 * This typically involves renaming its associated runtime edge to prevent conflicts.
	 */
	virtual void PrepareForCopying() override;
	//~ End UEdGraphNode Interface

	//~ Public Virtual Methods (Potential common interface, not direct UEdGraphNode overrides)
	/**
	 * @brief Gets the input pin of this edge node.
	 * @return A pointer to the input UEdGraphPin.
	 */
	virtual UEdGraphPin* GetInputPin() const { return Pins[0]; }

	/**
	 * @brief Gets the output pin of this edge node.
	 * @return A pointer to the output UEdGraphPin.
	 */
	virtual UEdGraphPin* GetOutputPin() const { return Pins[1]; }
	//~ End Public Virtual Methods

	//~ Public Methods
	/**
	 * @brief Creates connections for this edge between a start and end graph node.
	 * Modifies the pins of this edge and the connected start/end nodes.
	 * @param Start The graph node from which the edge originates.
	 * @param End The graph node to which the edge terminates.
	 */
	void CreateConnections(const UCrimsonSkillTree_GraphNode* Start, const UCrimsonSkillTree_GraphNode* End);

	/**
	 * @brief Gets the graph node at the start of this edge (where the edge originates).
	 * @return A pointer to the starting UCrimsonSkillTree_GraphNode, or nullptr if not connected.
	 */
	UCrimsonSkillTree_GraphNode* GetStartNode();

	/**
	 * @brief Gets the graph node at the end of this edge (where the edge terminates).
	 * @return A pointer to the ending UCrimsonSkillTree_GraphNode, or nullptr if not connected.
	 */
	UCrimsonSkillTree_GraphNode* GetEndNode();

	/**
	 * @brief Sets the runtime UCrimsonSkillTree_Edge object that this graph edge represents.
	 * @param Edge Pointer to the runtime edge data.
	 */
	void SetEdge(UCrimsonSkillTree_Edge* Edge);
	//~ End Public Methods
};