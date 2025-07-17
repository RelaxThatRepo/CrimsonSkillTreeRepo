// Copyright Crimson Sword Studio, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CrimsonSkillTree/Nodes/CrimsonSkillTree_Node.h" // For ENodeState

#include "CrimsonSkillTreeWidget_Node.generated.h"

class UCrimsonSkillTreeWidget_Display;
class UCrimsonSkillTreeWidget_Graph;
class UCrimsonSkillTreeWidget_NodeTooltip;
class UImage;
class UTextBlock;

/**
 * @class UCrimsonSkillTreeWidget_Node
 * @brief The UMG widget representing a single node in the skill tree graph.
 * This widget is reactive to its underlying data source (UCrimsonSkillTree_Node).
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class CRIMSONSKILLTREE_API UCrimsonSkillTreeWidget_Node : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Initializes the widget with its corresponding data node.
	 * This function binds the widget to the node's OnNodeStateChanged delegate, making it reactive.
	 * @param InNodeData The data object that this widget will represent.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Tree Node Widget")
	void InitializeNode(UCrimsonSkillTree_Node* InNodeData);
	virtual void InitializeNode_Implementation(UCrimsonSkillTree_Node* InNodeData);

	/** Getter for the skill node data */
	UFUNCTION(BlueprintPure, Category = "Skill Tree Node Widget")
	UCrimsonSkillTree_Node* GetSkillNodeData() const { return SkillNodeData; }

	/** Sets the owning graph widget, establishing the ownership chain. */
	void SetOwningGraphWidget(UCrimsonSkillTreeWidget_Graph* InOwningGraph);

	bool IsDoneInitializing() const;

protected:
	//~ UUserWidget Overrides
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeDestruct() override;
	//~ End UUserWidget Overrides

	//~ Blueprint Events
	/**
	 * @brief Blueprint-implementable event called to update the node's visuals based on its current state.
	 * @param CurrentNodeState The new state of the node.
	 * @param PreviousNodeState The state the node was in before the change.
	 * @param InCurrentLevel The new level of the node.
	 * @param InMaxLevel The maximum level of the node.
	 * @param bInIsRootNode True if this is the root node of the tree.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill Tree Node Widget", meta=(DisplayName="On Update Node Appearance"))
	void UpdateNodeAppearance(ENodeState CurrentNodeState, ENodeState PreviousNodeState, int32 InCurrentLevel, int32 InMaxLevel, bool bInIsRootNode);
	//~ End Blueprint Events

private:
	/**
	 * @brief The callback function that is executed when the underlying SkillNodeData's state changes.
	 * This function triggers the visual update of the widget.
	 */
	UFUNCTION()
	void OnNodeDataChanged();
	

protected:
	//~ UMG Widget Bindings
	// These are examples of UMG elements you might bind in your Blueprint subclass.
	UPROPERTY(BlueprintReadOnly, Category = "Visuals", meta = (BindWidgetOptional))
	TObjectPtr<UImage> NodeIcon;

	UPROPERTY(BlueprintReadOnly, Category = "Visuals", meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> NodeLevelText;
	//~ End UMG Widget Bindings
	
	//~ Configuration Properties
	// The tooltip widget class to spawn when this node is hovered.
	UPROPERTY(EditDefaultsOnly, Category = "Tooltip")
	TSubclassOf<UCrimsonSkillTreeWidget_NodeTooltip> TooltipWidgetClass;
	//~ End Configuration Properties

private:
	//~ Runtime State
	// The underlying skill node data object this widget represents.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill Tree Node Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCrimsonSkillTree_Node> SkillNodeData;

	// Cached weak pointer to the parent graph widget for efficient access.
	UPROPERTY(Transient)
	TWeakObjectPtr<UCrimsonSkillTreeWidget_Graph> OwningGraphWidget;

	UPROPERTY(Transient)
	ENodeState PreviousState;

	bool bIsDoneInitializing = false;
	//~ End Runtime State
};
