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
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~UUserWidget Overrides
	// =============================================================================================================
	/**
	 * @brief Called when the widget is constructed.
	 */
	virtual void NativeConstruct() override;

	// ~Public Methods
	// =============================================================================================================
	/**
	 * @brief Clears the existing graph and populates it with nodes and connections from a new skill tree asset.
	 * @param InSkillTree The skill tree instance to display.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Graph")
	virtual void PopulateGraph(UCrimsonSkillTree* InSkillTree);

	/**
	 * @brief Removes all node and line widgets from the graph.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Graph")
	virtual void ClearGraph();

	/**
	 * @brief Redraws all connection lines between nodes. Should be called when a node's state or position changes.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Graph")
	void RefreshConnections();

	// ~View Control
	// =============================================================================================================
	/**
	 * @brief Adjusts the graph's translation and scale to center the content within the viewport.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Graph|View")
	void RecenterViewOnGraphContent();

	/**
	 * @brief Adjusts the graph's translation and scale to center the Root Node within the viewport.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Graph|View")
	void RecenterViewOnRootNode();

	/**
	 * @brief Adjusts the graph's translation and scale to center on a specific node.
	 * @param NodeToCenter The node to center the view on.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Graph|View")
	void RecenterViewOnNode(UCrimsonSkillTree_Node* NodeToCenter);

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

	// ~Context & Accessors
	// =============================================================================================================
	/**
	 * @brief Sets the parent display widget that owns this graph.
	 * @param InDisplayWidget The parent display widget.
	 */
	void SetParentDisplayWidget(TWeakObjectPtr<UCrimsonSkillTreeWidget_Display> InDisplayWidget);

	/**
	 * @brief Gets the parent display widget that owns this graph.
	 * @return The parent display widget.
	 */
	UCrimsonSkillTreeWidget_Display* GetParentDisplayWidget() const { return ParentDisplayWidget.Get(); }

	/**
	 * @brief Gets the canvas panel that holds the node widgets.
	 * @return The node canvas panel.
	 */
	UCanvasPanel* GetNodeCanvasPanel() const { return NodeCanvasPanel; }

	// ~Debug
	// =============================================================================================================
	/**
	 * @brief Blueprint event for debugging zoom calculations.
	 * @param NewScale The new scale of the canvas.
	 * @param NewTranslation The new translation of the canvas.
	 * @param NewZoomOriginLocalSpace The local space origin of the zoom.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill Tree Graph|Debug")
	void GetZoomFactors(const FVector2D& NewScale, const FVector2D& NewTranslation, const FVector2D& NewZoomOriginLocalSpace) const;

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief Event broadcast when the graph has been fully populated and all connection lines have been drawn. */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGraphReady);
	UPROPERTY(BlueprintAssignable, Category = "Skill Tree Graph|Events")
	FOnGraphReady OnGraphReady;

protected:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~UUserWidget Overrides
	// =============================================================================================================
	/**
	 * @brief Called after the widget has been constructed.
	 */
	virtual void NativeOnInitialized() override;

#if WITH_EDITOR
	/**
	 * @brief Called in the editor when a property is changed to allow for immediate visual updates.
	 */
	virtual void SynchronizeProperties() override;
#endif

	// ~Widget Creation
	// =============================================================================================================
	/**
	 * @brief Creates a node widget instance based on the node data and configured class map.
	 * @param ForNodeData The data object for the node to be created.
	 * @return The newly created and initialized node widget.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Tree Graph")
	UCrimsonSkillTreeWidget_Node* CreateNodeWidget(UCrimsonSkillTree_Node* ForNodeData);
	virtual UCrimsonSkillTreeWidget_Node* CreateNodeWidget_Implementation(UCrimsonSkillTree_Node* ForNodeData);

	/**
	 * @brief Creates a visual node widget instance from its data.
	 * @param ForNodeData The data object for the visual node to be created.
	 * @return The newly created and initialized visual node widget.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Tree Graph")
	UCrimsonSkillTreeWidget_VisualNode* CreateVisualNodeWidget(UCrimsonSkillTree_VisualNode* ForNodeData);
	virtual UCrimsonSkillTreeWidget_VisualNode* CreateVisualNodeWidget_Implementation(UCrimsonSkillTree_VisualNode* ForNodeData);

	// ~Internal Drawing & Refresh
	// =============================================================================================================
	/**
	 * @brief Removes all line widgets from the canvas.
	 */
	void ClearLines();

	/**
	 * @brief Periodically checks if all node widgets have valid geometry, then refreshes connections.
	 * @details This function is called by a timer and clears itself once the work is done.
	 */
	UFUNCTION()
	void DeferredRefreshConnections();

	/**
	 * @brief Calculates the bounding box that encompasses all node widgets in the graph.
	 * @return An FBox2D representing the total bounds of the content in local canvas space.
	 */
	FBox2D GetGraphContentBounds() const;

	/**
	 * @brief Handles the UI update delegate from the manager when replicated data changes.
	 */
	UFUNCTION()
	void HandleSkillTreeStateUpdated();

protected:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/

	// ~UMG Widget Bindings
	// =============================================================================================================
	/** @brief The Canvas Panel that holds all the node widgets. Must be bound in the UMG editor. */
	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Graph|Visuals", meta = (BindWidget))
	TObjectPtr<UCanvasPanel> NodeCanvasPanel;

	// ~Configuration Properties
	// =============================================================================================================
	/** @brief The default widget class to use for nodes if no specific class is found in the NodeTypeToWidgetClassMap. */
	UPROPERTY(EditDefaultsOnly, Category = "Skill Tree Graph|Configuration")
	TSubclassOf<UCrimsonSkillTreeWidget_Node> DefaultNodeWidgetClass;

	/** @brief Maps a node's Gameplay Tag to a specific widget class, allowing for different visual styles for different node types. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree Graph|Configuration")
	TMap<FGameplayTag, TSubclassOf<UCrimsonSkillTreeWidget_Node>> NodeTypeToWidgetClassMap;

	/** @brief The line drawing policy object that defines the appearance and logic for drawing connection lines. */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Skill Tree Graph|Configuration")
	TObjectPtr<UCrimsonSkillTreeWidget_LineDrawingPolicyBase> LineDrawingPolicy;

	/** @brief The starting "Zoom" distance for the skill tree. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Tree Graph|Configuration|Zoom")
	FVector2D StartingRenderScale = FVector2D(1.5f, 1.5f);

	/** @brief How fast you can zoom in/out. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Tree Graph|Configuration|Zoom")
	float ZoomSpeedFactor = 0.07f;

	/** @brief The minimum allowed zoom level. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Tree Graph|Configuration|Zoom")
	float MinZoomDistance = 0.75f;

	/** @brief The maximum allowed zoom level. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Skill Tree Graph|Configuration|Zoom")
	float MaxZoomDistance = 1.75f;

private:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/

	// ~Runtime State
	// =============================================================================================================
	/** @brief The skill tree asset currently being displayed. */
	UPROPERTY(Transient)
	TObjectPtr<UCrimsonSkillTree> SkillTreeAsset;

	/** @brief A map to quickly access a node's UMG widget from its data object. */
	UPROPERTY(Transient)
	TMap<UCrimsonSkillTree_Node*, UCrimsonSkillTreeWidget_Node*> NodeWidgetMap;

	/** @brief A map to quickly access a visual node's UMG widget from its data object. */
	UPROPERTY(Transient)
	TMap<UCrimsonSkillTree_VisualNode*, UCrimsonSkillTreeWidget_VisualNode*> VisualNodeWidgetMap;

	/** @brief An array holding all the UImage widgets used to draw connection lines. */
	UPROPERTY(Transient)
	TArray<UImage*> DrawnLineWidgets;

	/** @brief Weak pointer to the owning display widget to avoid circular dependencies. */
	UPROPERTY(Transient)
	TWeakObjectPtr<UCrimsonSkillTreeWidget_Display> ParentDisplayWidget;

	/** @brief Timer handle for the deferred connection refresh logic. */
	FTimerHandle ConnectionRefreshTimerHandle;

	/** @brief Flag to ensure the OnGraphReady event is only broadcast once per population. */
	bool bIsInitialGraphReady = false;
};
