// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// UEdGraphNode is implicitly included via .generated.h or CoreMinimal for UCLASS derivation
#include "CrimsonSkillTree_GraphNode.generated.h"

class UCrimsonSkillTree_Graph; // Original forward declaration
class UCrimsonSkillTree_Node;  // Original forward declaration
class SCrimsonSkillTree_GraphNode; // Original forward declaration
class UEdGraphSchema; // Forward declaration for parameter type
class UEdGraphPin;    // Forward declaration for return/parameter types

/**
 * @class UCrimsonSkillTree_GraphNode
 * @brief Represents a node in the editor graph for a Crimson Skill Tree.
 * This class is the UEdGraphNode counterpart to the runtime UCrimsonSkillTree_Node.
 * It handles editor-specific visuals, pin management, and interactions within the graph editor.
 */
UCLASS(MinimalAPI) // Preserving original MinimalAPI
class UCrimsonSkillTree_GraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	//~ Properties
	/** @brief Pointer to the runtime UCrimsonSkillTree_Node object that this graph node represents and configures. */
	UPROPERTY(VisibleAnywhere, Instanced, Category = "Skill Tree Node") // Added category for clarity
	TObjectPtr<UCrimsonSkillTree_Node> SkillTreeNode;

	/** @brief Pointer to the Slate graph node widget (SGraphNode) that visually represents this node in the editor. This is a runtime editor-only link. */
	SCrimsonSkillTree_GraphNode* SGraphNode; // This is a raw pointer to an SWidget, typically managed by the graph editor UI.
	//~ End Properties

	/**
	 * @brief Default constructor.
	 */
	UCrimsonSkillTree_GraphNode();

	//~ UObject Interface
	/**
	 * @brief Destructor.
	 */
	virtual ~UCrimsonSkillTree_GraphNode();

#if WITH_EDITOR
	/**
	 * @brief Called after an undo or redo transaction that affected this node.
	 * Allows the node to refresh its state or visuals.
	 */
	virtual void PostEditUndo() override;
#endif
	//~ End UObject Interface

	//~ Public Methods
	/**
	 * @brief Sets the runtime UCrimsonSkillTree_Node data object that this graph node represents.
	 * @param InSkillTreeNode Pointer to the runtime skill tree node data.
	 */
	void SetSkillTreeNode(UCrimsonSkillTree_Node* InSkillTreeNode);

	/**
	 * @brief Gets the UCrimsonSkillTree_Graph that this node belongs to.
	 * @return A pointer to the owning skill tree graph.
	 */
	UCrimsonSkillTree_Graph* GetSkillTreeGraph();
	
	/**
	 * @brief Gets the visual background color for this node in the graph editor.
	 * This can be overridden by derived classes for different node types.
	 * @return The FLinearColor for the node's background.
	 */
	virtual FLinearColor GetBackgroundColor() const; // Not a UEdGraphNode override, but a common virtual for custom graph nodes

	/**
	 * @brief Gets the primary input pin of this node.
	 * @return A pointer to the input UEdGraphPin, or nullptr if none exists.
	 */
	virtual UEdGraphPin* GetInputPin() const; // Not a UEdGraphNode override

	/**
	 * @brief Gets the primary output pin of this node.
	 * @return A pointer to the output UEdGraphPin, or nullptr if none exists.
	 */
	virtual UEdGraphPin* GetOutputPin() const; // Not a UEdGraphNode override

	/**
	 * @brief Gets the current visual position of the node in the graph editor.
	 * @return An FVector2D representing the node's X and Y coordinates.
	 */
	FVector2D GetNodePosition() const;
	//~ End Public Methods

	//~ UEdGraphNode Interface
	/**
	 * @brief Allocates the default input and output pins for this node.
	 * Called when the node is created or needs its pins reconstructed.
	 */
	virtual void AllocateDefaultPins() override;

	/**
	 * @brief Gets the title of this node for display in the graph editor.
	 * @param TitleType The type of title requested (e.g., full, editable).
	 * @return The title text, usually derived from the associated SkillTreeNode.
	 */
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	/**
	 * @brief Prepares this graph node for copying.
	 * This often involves duplicating or appropriately referencing its associated runtime data (SkillTreeNode).
	 */
	virtual void PrepareForCopying() override;

	/**
	 * @brief Called when a new node is created and an attempt is made to automatically wire it from a source pin.
	 * @param FromPin The pin from which the connection is being initiated.
	 */
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;

	/**
	 * @brief Checks if this node type can be created under the specified graph schema.
	 * @param Schema The graph schema to check against.
	 * @return True if the node can be created under the schema, false otherwise.
	 */
	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const override;
	//~ End UEdGraphNode Interface
};