#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CrimsonSkillTree_Edge.generated.h"

class UCrimsonSkillTree;
class UCrimsonSkillTree_Node;

/**
 * @class UCrimsonSkillTree_Edge
 * @brief Represents a connection between two nodes in a skill tree graph.
 */
UCLASS(Blueprintable)
class CRIMSONSKILLTREE_API UCrimsonSkillTree_Edge : public UObject
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Construction
	// =============================================================================================================
	UCrimsonSkillTree_Edge();
	virtual ~UCrimsonSkillTree_Edge();

	// ~Public Accessors
	// =============================================================================================================
	/**
	 * @brief Gets the skill tree this edge belongs to.
	 * @return The owning skill tree object.
	 */
	UFUNCTION(BlueprintPure, Category = "Crimson|SkillTree")
	UCrimsonSkillTree* GetSkillTree();

#if WITH_EDITOR
	// ~Editor Methods
	// =============================================================================================================
	/**
	 * @brief Gets the title of the node (edge).
	 * @return The node title text.
	 */
	virtual FText GetNodeTitle() const { return NodeTitle; }

	/**
	 * @brief Gets the color of the edge for graph editor display.
	 * @return The linear color of the edge.
	 */
	FLinearColor GetEdgeColor() const { return EdgeColor; }

	/**
	 * @brief Sets the title of the node (edge).
	 * @param NewTitle The new title text.
	 */
	virtual void SetNodeTitle(const FText& NewTitle);
#endif

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief The skill tree this edge is a part of. */
	UPROPERTY()
	TObjectPtr<UCrimsonSkillTree> SkillTree;

	/** @brief The node where this edge originates. */
	UPROPERTY(BlueprintReadOnly, Category = "Crimson|SkillTree|Edge")
	TObjectPtr<UCrimsonSkillTree_Node> StartNode;

	/** @brief The node where this edge terminates. */
	UPROPERTY(BlueprintReadOnly, Category = "Crimson|SkillTree|Edge")
	TObjectPtr<UCrimsonSkillTree_Node> EndNode;

#if WITH_EDITORONLY_DATA
	/** @brief Whether the title should be drawn on the edge in the graph editor. */
	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode_Editor")
	bool bShouldDrawTitle = false;

	/** @brief The title text for the edge, if displayed. */
	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode_Editor")
	FText NodeTitle;

	/** @brief The color of the edge in the graph editor. */
	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphEdge")
	FLinearColor EdgeColor = FLinearColor(0.6f, 0.0f, 0.0f, 0.3f);
#endif
};
