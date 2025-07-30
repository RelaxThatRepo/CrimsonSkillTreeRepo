#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "InputMappingContext.h"
#include "CrimsonSkillTree/Nodes/CrimsonSkillTree_Node.h"
#include "CrimsonSkillTree/CrimsonSkillTreeManager.h"
#include "CrimsonSkillTreeWidget_Node.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UCrimsonSkillTreeWidget_Display;
class UCrimsonSkillTreeWidget_Graph;
class UCrimsonSkillTreeWidget_NodeTooltip;
class UImage;
class UTextBlock;

/**
 * @class UCrimsonSkillTreeWidget_Node
 * @brief The UMG widget representing a single node in the skill tree graph.
 * @details This widget is reactive to its underlying data source (UCrimsonSkillTree_Node).
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class CRIMSONSKILLTREE_API UCrimsonSkillTreeWidget_Node : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Initialization & Context
	// =============================================================================================================
	/**
	 * @brief Initializes the widget with its corresponding data node.
	 * @details This function binds the widget to the node's OnNodeStateChanged delegate, making it reactive.
	 * @param InNodeData The data object that this widget will represent.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Tree Node Widget")
	void InitializeNode(UCrimsonSkillTree_Node* InNodeData);
	virtual void InitializeNode_Implementation(UCrimsonSkillTree_Node* InNodeData);

	/**
	 * @brief Gets the underlying skill node data object.
	 * @return The skill node data.
	 */
	UFUNCTION(BlueprintPure, Category = "Skill Tree Node Widget")
	UCrimsonSkillTree_Node* GetSkillNodeData() const { return SkillNodeData; }

	/**
	 * @brief Sets the owning graph widget, establishing the ownership chain.
	 * @param InOwningGraph The parent graph widget.
	 */
	void SetOwningGraphWidget(UCrimsonSkillTreeWidget_Graph* InOwningGraph);

	/**
	 * @brief Checks if the widget has completed its initial setup.
	 * @return True if initialization is complete.
	 */
	bool IsDoneInitializing() const;

	// ~Actions
	// =============================================================================================================
	/**
	 * @brief Requests the server to perform an action on this node (e.g., activate, deactivate).
	 * @details This is the primary function your UI should call from Blueprint on player input.
	 * @param ActionType The action to perform.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Node Widget|Actions")
	void RequestNodeAction(ECrimsonSkillNodeActionType ActionType);

	// ~Event Handlers
	// =============================================================================================================
	/**
	 * @brief The callback function that is executed when the underlying SkillNodeData's state changes.
	 * @details This function triggers the visual update of the widget.
	 */
	UFUNCTION()
	void OnNodeDataChanged();

protected:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~UUserWidget Overrides
	// =============================================================================================================
	/**
	 * @brief Called when the mouse pointer enters the widget's bounds.
	 * @param InGeometry The geometry of the widget.
	 * @param InMouseEvent The mouse event data.
	 */
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	/**
	 * @brief Called when the mouse pointer leaves the widget's bounds.
	 * @param InMouseEvent The mouse event data.
	 */
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	/**
	 * @brief Called when the widget is being destroyed.
	 */
	virtual void NativeDestruct() override;

	// ~Blueprint Events
	// =============================================================================================================
	/**
	 * @brief Blueprint-implementable event called to update the node's visuals based on its current state.
	 * @param CurrentNodeState The new state of the node.
	 * @param PreviousNodeState The state the node was in before the change.
	 * @param InCurrentLevel The new level of the node.
	 * @param InMaxLevel The maximum level of the node.
	 * @param bInIsRootNode True if this is the root node of the tree.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill Tree Node Widget", meta = (DisplayName = "On Update Node Appearance"))
	void UpdateNodeAppearance(ENodeState CurrentNodeState, ENodeState PreviousNodeState, int32 InCurrentLevel, int32 InMaxLevel, bool bInIsRootNode);

private:
	bool IsKeyMappedToAction(const FKey& Key, const UInputAction* Action,
	                         const UInputMappingContext* MappingContext) const;

protected:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/

	// ~Input Properties
	// =============================================================================================================
	/** @brief Reference to the parent display widget for accessing input actions. */
	UPROPERTY(Transient)
	TWeakObjectPtr<UCrimsonSkillTreeWidget_Display> ParentDisplayWidget;
	
	// ~UMG Widget Bindings
	// =============================================================================================================
	/** @brief Optional binding for an image widget representing the node's icon. */
	UPROPERTY(BlueprintReadOnly, Category = "Visuals", meta = (BindWidgetOptional))
	TObjectPtr<UImage> NodeIcon;

	/** @brief Optional binding for a text block widget to display the node's level. */
	UPROPERTY(BlueprintReadOnly, Category = "Visuals", meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> NodeLevelText;

	// ~Configuration Properties
	// =============================================================================================================
	/** @brief The tooltip widget class to spawn when this node is hovered. */
	UPROPERTY(EditDefaultsOnly, Category = "Tooltip")
	TSubclassOf<UCrimsonSkillTreeWidget_NodeTooltip> TooltipWidgetClass;

private:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief The underlying skill node data object this widget represents. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Skill Tree Node Widget", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCrimsonSkillTree_Node> SkillNodeData;

	/** @brief Cached weak pointer to the parent graph widget for efficient access. */
	UPROPERTY(Transient)
	TWeakObjectPtr<UCrimsonSkillTreeWidget_Graph> OwningGraphWidget;

	/** @brief The state of the node before the last update. */
	UPROPERTY(Transient)
	ENodeState PreviousState;

	/** @brief Flag to indicate if the widget has been initialized. */
	bool bIsDoneInitializing = false;
};
