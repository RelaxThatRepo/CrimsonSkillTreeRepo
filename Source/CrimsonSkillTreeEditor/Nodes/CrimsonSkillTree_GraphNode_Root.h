// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTree_GraphNode.h" // Original include
#include "CrimsonSkillTree_GraphNode_Root.generated.h"

/**
 * @class UCrimsonSkillTree_GraphNode_Root
 * @brief Represents the root node in the editor graph for a Crimson Skill Tree.
 * This specialized graph node has distinct visual properties and pin configurations
 * appropriate for a root node (e.g., only an output pin).
 */
UCLASS() // Preserving original UCLASS() macro as provided
class CRIMSONSKILLTREEEDITOR_API UCrimsonSkillTree_GraphNode_Root : public UCrimsonSkillTree_GraphNode
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor. Initializes root node specific properties.
	 */
	UCrimsonSkillTree_GraphNode_Root();
	
	//~ UCrimsonSkillTree_GraphNode Interface (and UEdGraphNode Overrides)
	/**
	 * @brief Gets the title of this root node for display in the graph editor.
	 * Root nodes typically have a fixed title.
	 * @param TitleType The type of title requested (e.g., full, editable).
	 * @return The static title text "ROOT".
	 */
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	/**
	 * @brief Gets the tooltip text for this root node.
	 * Delegates to the base UEdGraphNode implementation.
	 * @return The tooltip text.
	 */
	virtual FText GetTooltipText() const override; // Override from UEdGraphNode

	/**
	 * @brief Gets the background color for this root node in the graph editor.
	 * Allows for visual distinction of the root node.
	 * @return The FLinearColor for the root node's background.
	 */
	virtual FLinearColor GetBackgroundColor() const override; // Override from UCrimsonSkillTree_GraphNode

	/**
	 * @brief Allocates the default pins for the root node.
	 * Root nodes typically only have an output pin and no input pins.
	 */
	virtual void AllocateDefaultPins() override; // Override from UEdGraphNode
	//~ End UCrimsonSkillTree_GraphNode Interface

	//~ UObject Interface (via UEdGraphNode)
#if WITH_EDITOR // This guard is often for UObject methods conditionally compiled
	/**
	 * @brief Called after an undo or redo transaction that affected this node.
	 * Allows the node to refresh its state or visuals.
	 */
	virtual void PostEditUndo() override;
	//~ End UObject Interface
#endif // WITH_EDITOR
};