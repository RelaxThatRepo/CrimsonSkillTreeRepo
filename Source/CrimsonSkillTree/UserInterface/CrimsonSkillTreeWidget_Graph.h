// Copyright Crimson Sword Studio, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "CrimsonSkillTreeWidget_Graph.generated.h"

class UCrimsonSkillTree_VisualNode;
class UCrimsonSkillTreeWidget_VisualNode;
class UCrimsonSkillTree;
class UCrimsonSkillTree_Node;
class UCrimsonSkillTreeWidget_Node;
class UCrimsonSkillTreeWidget_Display;
class UCrimsonSkillTreeWidget_LineDrawingPolicyBase;
class UCanvasPanel;
class UImage;

/**
 * @class UCrimsonSkillTreeWidget_Graph
 * @brief Manages the visual representation of a skill tree, including nodes and connecting lines, within a pannable, zoomable canvas.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class CRIMSONSKILLTREE_API UCrimsonSkillTreeWidget_Graph : public UCommonUserWidget
{
    GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	//~ Public Methods
	/**
	 * @brief Clears the existing graph and populates it with nodes and connections from a new skill tree asset.
	 * @param InSkillTree The skill tree instance to display.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Graph")
	void PopulateGraph(UCrimsonSkillTree* InSkillTree);

	/**
	 * @brief Removes all node and line widgets from the graph.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Graph")
	void ClearGraph();

	/**
	 * @brief Adjusts the graph's translation and scale to center the content within the viewport.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Graph")
	void RecenterViewOnGraphContent();

	/**
	 * @brief Redraws all connection lines between nodes. Should be called when a node's state or position changes.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Graph")
	void RefreshConnections();
	
	/**
	 * @brief Pans the node canvas based on mouse movement.
	 * @param CurrentMouse_ViewportSpace The current mouse position in absolute viewport coordinates.
	 * @param PanStartMouse_ViewportSpace The mouse position where panning began.
	 * @param InitialCanvasTranslation The canvas's render translation when panning began.
	 */
	void PanCanvasBasedOnMouse(const FVector2D& CurrentMouse_ViewportSpace, const FVector2D& PanStartMouse_ViewportSpace, const FVector2D& InitialCanvasTranslation);
	
	/**
	 * @brief Applies zoom to the node canvas.
	 * @param WheelDelta The delta from the mouse wheel input.
	 * @param MousePositionAbsolute The position to zoom towards, in absolute screen space. If not provided, zooms to the center of the widget.
	 */
	void ApplyZoom(float WheelDelta, TOptional<FVector2D> MousePositionAbsolute = TOptional<FVector2D>());

	/** Setters and Getters for ownership chain */
	void SetParentDisplayWidget(TWeakObjectPtr<UCrimsonSkillTreeWidget_Display> InDisplayWidget);
	UCrimsonSkillTreeWidget_Display* GetParentDisplayWidget() const { return ParentDisplayWidget.Get(); }
	UCanvasPanel* GetNodeCanvasPanel() const { return NodeCanvasPanel; }

	//Debug Methods
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill Tree Graph|Debug")
	void GetZoomFactors(const FVector2D& NewScale, const FVector2D& NewTranslation, const FVector2D& NewZoomOriginLocalSpace) const;
	
	//~ End Public Methods

protected:
	//~ UUserWidget Overrides
	virtual void NativeOnInitialized() override;

#if WITH_EDITOR
	virtual void SynchronizeProperties() override;
#endif
	//~ End UUserWidget Overrides

	//~ Protected Methods
	/**
	 * @brief Creates a node widget instance based on the node data and configured class map.
	 * @param ForNodeData The data object for the node to be created.
	 * @return The newly created and initialized node widget.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Tree Graph")
	UCrimsonSkillTreeWidget_Node* CreateNodeWidget(UCrimsonSkillTree_Node* ForNodeData);
	virtual UCrimsonSkillTreeWidget_Node* CreateNodeWidget_Implementation(UCrimsonSkillTree_Node* ForNodeData);


	UFUNCTION(BlueprintNativeEvent, Category = "Skill Tree Graph")
	UCrimsonSkillTreeWidget_VisualNode* CreateVisualNodeWidget(UCrimsonSkillTree_VisualNode* ForNodeData);
	virtual UCrimsonSkillTreeWidget_VisualNode* CreateVisualNodeWidget_Implementation(UCrimsonSkillTree_VisualNode* ForNodeData);
	/**
	 * @brief Removes all line widgets from the canvas.
	 */
	void ClearLines();

	/**
 	* @brief Periodically checks if all node widgets have valid geometry, then refreshes connections.
 	* This function is called by a timer and clears itself once the work is done.
 	*/
	UFUNCTION()
	void DeferredRefreshConnections();

	/**
	 * @brief Calculates the bounding box that encompasses all node widgets in the graph.
	 * @return An FBox2D representing the total bounds of the content in local canvas space.
	 */
	FBox2D GetGraphContentBounds() const;
	//~ End Protected Methods

public:
	
	/** Event broadcast when the graph has been fully populated and all connection lines have been drawn. */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGraphReady);
	UPROPERTY(BlueprintAssignable, Category = "Skill Tree Graph|Events")
	FOnGraphReady OnGraphReady;
	
protected:
	//~ UMG Widget Bindings
	// The Canvas Panel that holds all the node widgets. Must be bound in the UMG editor.
	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Graph|Visuals", meta = (BindWidget))
	TObjectPtr<UCanvasPanel> NodeCanvasPanel;
	//~ End UMG Widget Bindings

	//~ Configuration Properties
	// The default widget class to use for nodes if no specific class is found in the NodeTypeToWidgetClassMap.
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree Graph|Configuration")
	TSubclassOf<UCrimsonSkillTreeWidget_Node> DefaultNodeWidgetClass;

	// Maps a node's Gameplay Tag to a specific widget class, allowing for different visual styles for different node types.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree Graph|Configuration")
	TMap<FGameplayTag, TSubclassOf<UCrimsonSkillTreeWidget_Node>> NodeTypeToWidgetClassMap;
	
	// The line drawing policy object that defines the appearance and logic for drawing connection lines.
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Skill Tree Graph|Configuration")
	TObjectPtr<UCrimsonSkillTreeWidget_LineDrawingPolicyBase> LineDrawingPolicy;

	// The starting "Zoom" distance for the skill tree
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Tree Graph|Configuration|Zoom")
	FVector2D StartingRenderScale = FVector2D(1.5f, 1.5f);

	// How fast you can zoom in/out
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Tree Graph|Configuration|Zoom")
	float ZoomSpeedFactor = 0.07f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Tree Graph|Configuration|Zoom")
	float MinZoomDistance = 0.75f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Tree Graph|Configuration|Zoom")
	float MaxZoomDistance = 1.75f;
	//~ End Configuration Properties

private:
	//~ Runtime State
	// The skill tree asset currently being displayed.
	UPROPERTY(Transient)
	TObjectPtr<UCrimsonSkillTree> SkillTreeAsset;

	// A map to quickly access a node's UMG widget from its data object.
    UPROPERTY(Transient)
    TMap<UCrimsonSkillTree_Node*, UCrimsonSkillTreeWidget_Node*> NodeWidgetMap;

	// A map to quickly access a node's UMG widget from its data object.
	UPROPERTY(Transient)
	TMap<UCrimsonSkillTree_VisualNode*, UCrimsonSkillTreeWidget_VisualNode*> VisualNodeWidgetMap;

	// An array holding all the UImage widgets used to draw connection lines.
	UPROPERTY(Transient)
    TArray<UImage*> DrawnLineWidgets;

	// Weak pointer to the owning display widget to avoid circular dependencies.
	UPROPERTY(Transient)
	TWeakObjectPtr<UCrimsonSkillTreeWidget_Display> ParentDisplayWidget;

	// Timer handle for the deferred connection refresh logic.
	FTimerHandle ConnectionRefreshTimerHandle;

	// Flag to ensure the OnGraphReady event is only broadcast once per population.
	bool bIsInitialGraphReady = false;
	//~ End Runtime State
};
