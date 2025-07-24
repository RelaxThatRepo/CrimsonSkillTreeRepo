#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CrimsonSkillTree_VisualNode.generated.h"

class UCrimsonSkillTree;
class UCommonBorderStyle;
class UCommonTextStyle;
class UCrimsonSkillTreeWidget_VisualNode;

/**
 * @struct FConditionIndexArray
 * @brief A wrapper struct to hold an array of integers, allowing it to be used as a value in a TMap UPROPERTY.
 */
USTRUCT(BlueprintType)
struct FConditionIndexArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int32> Indices;
};

/**
 * @class UCrimsonSkillTree_VisualNode
 * @brief Represents a non-functional, decorative node in the skill tree graph editor, such as a comment box or a frame.
 */
UCLASS(Blueprintable)
class CRIMSONSKILLTREE_API UCrimsonSkillTree_VisualNode : public UObject
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/
	/**
	 * @brief Sets the owning skill tree for this visual node.
	 * @param InOwningGraph The skill tree that this visual node belongs to.
	 */
	UFUNCTION(BlueprintCallable, Category = "Visual Node")
	void SetOwningSkillTree(UCrimsonSkillTree* InOwningGraph);

	/**
	 * @brief Gets the owning skill tree for this visual node.
	 * @return The skill tree that this visual node belongs to.
	 */
	UFUNCTION(BlueprintCallable, Category = "Visual Node")
	UCrimsonSkillTree* GetOwningSkillTree() const { return OwningGraph; };

	/**
	 * @brief Gets the map of node GUIDs to the condition indices they reference.
	 * @return A const reference to the node condition reference map.
	 */
	UFUNCTION(BlueprintCallable, Category = "Visual Node")
	const TMap<FGuid, FConditionIndexArray>& GetNodeConditionReferences() const { return NodeConditionReference; }

	/**
	 * @brief Gets the tooltip description for a specific condition referenced by this visual node.
	 * @param NodeId The GUID of the skill node that owns the condition.
	 * @param ConditionIndex The index of the condition in the node's ActivationConditions array.
	 * @return The tooltip description text for the specified condition.
	 */
	UFUNCTION(BlueprintCallable, Category = "Visual Node")
	FText GetNodeConditionTooltipDescription(const FGuid& NodeId, const int32 ConditionIndex) const;

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief The display title for this visual node. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Node")
	FText Title;

	/** @brief The position of the top-left corner of this node in the graph. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Node")
	FVector2D Position;

	/** @brief The size (width and height) of this node. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Node")
	FVector2D Size;

	/** @brief The widget class to spawn for this visual node in the game UI. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Node")
	TSubclassOf<UCrimsonSkillTreeWidget_VisualNode> WidgetClass;

	/** @brief Overrides the default text style for the visual node's widget. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Node|Overrides")
	TSubclassOf<UCommonTextStyle> TextStyle;

	/** @brief Overrides the default border style for the visual node's widget. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Node|Overrides")
	TSubclassOf<UCommonBorderStyle> BorderStyle;

	/** @brief If true, this visual node will attempt to display a description from a linked node's activation condition. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Node|Conditions")
	bool bUseConditionNodeDescription = false;

	/** @brief A reference to specific conditions on specific nodes, used to display their descriptions. The Key is the Node's GUID, and the Value is an array of condition indices on that node. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Node|Conditions", meta = (EditCondition = "bUseConditionNodeDescription"))
	TMap<FGuid, FConditionIndexArray> NodeConditionReference;

private:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief A pointer to the owning skill tree. */
	UPROPERTY()
	TObjectPtr<UCrimsonSkillTree> OwningGraph;
};
