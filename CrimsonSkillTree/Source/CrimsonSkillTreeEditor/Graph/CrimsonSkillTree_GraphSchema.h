// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// EdGraph/EdGraphSchema.h is implicitly included by .generated.h or CoreMinimal for UCLASS derivation
// No explicit user-added forward declarations for standard engine editor types will be added here.
// They are expected to be available from CoreMinimal.h or other standard editor includes.
#include "CrimsonSkillTree_GraphSchema.generated.h"

/**
 * @class FNodeVisitorCycleChecker
 * @brief Helper class to detect cycles in the graph when attempting to create new connections.
 * It performs a depth-first traversal to check if connecting two nodes would form a loop.
 */
class FNodeVisitorCycleChecker
{
public:
	/** * @brief Checks if creating a link from StartNode's output to EndNode's input would cause a cycle.
	 * @param StartNode The node from which the potential connection originates.
	 * @param EndNode The node to which the potential connection terminates.
	 * @return True if no cycle is detected (connection is safe), false if a cycle would be formed.
	 */
	bool CheckForLoop(UEdGraphNode* StartNode, UEdGraphNode* EndNode)
	{
		VisitedNodes.Add(StartNode);
		return TraverseNodes(EndNode);
	}

private:
	/**
	 * @brief Recursive traversal function used by CheckForLoop.
	 * @param Node The current node being visited in the traversal.
	 * @return True if traversal can continue without finding the initial StartNode (no cycle detected so far on this path), 
	 * false if the StartNode is encountered (cycle detected).
	 */
	bool TraverseNodes(UEdGraphNode* Node)
	{
		VisitedNodes.Add(Node);

		for (auto MyPin : Node->Pins)
		{
			if (MyPin->Direction == EGPD_Output)
			{
				for (auto OtherPin : MyPin->LinkedTo)
				{
					UEdGraphNode* OtherNode = OtherPin->GetOwningNode();
					if (VisitedNodes.Contains(OtherNode))
					{
						return false;
					}
					else if (!FinishedNodes.Contains(OtherNode))
					{
						if (!TraverseNodes(OtherNode))
							return false;
					}
				}
			}
		}

		VisitedNodes.Remove(Node);
		FinishedNodes.Add(Node);
		return true;
	};

	/** @brief Set of nodes currently in the recursion stack for the active traversal path. Used to detect back-edges. */
	TSet<TObjectPtr<UEdGraphNode>> VisitedNodes;
	/** @brief Set of nodes that have been fully traversed (all their children visited). Used to avoid redundant traversals. */
	TSet<TObjectPtr<UEdGraphNode>> FinishedNodes;
};

/**
 * @class UCrimsonSkillTree_GraphSchema
 * @brief Defines the rules and actions for a Crimson Skill Tree editor graph.
 * This schema controls aspects like what types of nodes can be created, how nodes can be connected,
 * context menu actions, and the visual style of connections.
 */
UCLASS(MinimalAPI) // Preserving original MinimalAPI
class UCrimsonSkillTree_GraphSchema: public UEdGraphSchema
{
	GENERATED_BODY()

public:
	//~ Public Methods (Not Overrides)
	/**
	 * @brief Populates a sub-menu with actions to break links to specific pins when a pin has multiple connections.
	 * This is a helper called by GetContextMenuActions.
	 * @param Menu The tool menu to populate.
	 * @param InGraphPin The pin for which to get break-link actions.
	 */
	void GetBreakLinkToSubMenuActions(class UToolMenu* Menu, class UEdGraphPin* InGraphPin);
	//~ End Public Methods (Not Overrides)

	//~ UEdGraphSchema Interface
	/**
	 * @brief Gets the type of the graph this schema is associated with.
	 * @param TestEdGraph The graph to test.
	 * @return EGraphType, defining the general behavior (e.g., state machine, ubergraph).
	 */
	virtual EGraphType GetGraphType(const UEdGraph* TestEdGraph) const override;

	/**
	 * @brief Populates the context menu when right-clicking on the graph background.
	 * Used to add actions like creating new nodes.
	 * @param ContextMenuBuilder Builder object to add context menu actions to.
	 */
 	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;

	/**
	 * @brief Populates the context menu when right-clicking on a graph node or pin.
	 * @param Menu The tool menu to populate with actions.
	 * @param Context Provides context about the clicked item (node, pin, etc.).
	 */
	virtual void GetContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

	/**
	 * @brief Determines if a connection can be created between two specified pins.
	 * @param A The first pin in the potential connection.
	 * @param B The second pin in the potential connection.
	 * @return An FPinConnectionResponse indicating whether the connection is allowed, disallowed, or requires conversion.
	 */
 	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;

	/**
	 * @brief Called to create the default set of nodes when a new graph is created (e.g., a root node).
	 * @param Graph The newly created UEdGraph instance to populate.
	 */
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;

	/**
	 * @brief Attempts to create a direct connection between two pins.
	 * This is called if CanCreateConnection returned a response allowing a direct make.
	 * @param A The first pin.
	 * @param B The second pin.
	 * @return True if the connection was successfully created, false otherwise.
	 */
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;

	/**
	 * @brief Called if CanCreateConnection suggests a conversion node is needed (e.g., an edge node).
	 * This function should create the intermediate node and connect it between the two pins.
	 * @param A The first pin.
	 * @param B The second pin.
	 * @return True if the conversion node and connections were successfully created, false otherwise.
	 */
	virtual bool CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const override;

	/**
	 * @brief Creates the drawing policy used to render connections (lines/wires) in the graph editor.
	 * @param InBackLayerID The layer ID for elements drawn behind nodes.
	 * @param InFrontLayerID The layer ID for elements drawn in front of nodes.
	 * @param InZoomFactor The current zoom level of the graph editor.
	 * @param InClippingRect The current clipping rectangle for drawing.
	 * @param InDrawElements The list of Slate draw elements to add to.
	 * @param InGraphObj The UEdGraph object being drawn.
	 * @return A new instance of a FConnectionDrawingPolicy derivative.
	 */
	virtual class FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const override;
 	
	/**
	 * @brief Gets the color to use for rendering a pin of a specific type.
	 * @param PinType The FEdGraphPinType structure describing the pin.
	 * @return The FLinearColor for the pin.
	 */
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;

	/**
	 * @brief Breaks all incoming and outgoing links from the specified graph node.
	 * @param TargetNode The node whose links are to be broken.
	 */
 
	virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;
	/**
	 * @brief Breaks all links connected to the specified pin.
	 * @param TargetPin The pin whose links are to be broken.
	 * @param bSendsNodeNotifcation If true, the owning node will be notified of the connection changes.
	 */
 	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotifcation) const override;

	/**
	 * @brief Breaks a single specific link between two pins.
	 * @param SourcePin The pin on one end of the link.
	 * @param TargetPin The pin on the other end of the link.
	 */
	virtual void BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const override;

	/**
	 * @brief Called when a pin is dropped onto a node in the graph editor.
	 * This function should determine which pin on the target node the dropped pin should connect to.
	 * @param InTargetNode The node onto which the pin was dropped.
	 * @param InSourcePinName The name of the pin being dropped.
	 * @param InSourcePinType The type of the pin being dropped.
	 * @param InSourcePinDirection The direction of the pin being dropped.
	 * @return The pin on InTargetNode that the source pin should connect to, or nullptr if no connection should be made.
	 */
	virtual UEdGraphPin* DropPinOnNode(UEdGraphNode* InTargetNode, const FName& InSourcePinName, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection) const override;
	
	/**
	 * @brief Determines if a pin of a given type and direction can be dropped onto a specified target node.
	 * @param InTargetNode The node being targeted for the drop.
	 * @param InSourcePinType The type of the pin being dropped.
	 * @param InSourcePinDirection The direction of the pin being dropped.
	 * @param OutErrorMessage If the drop is not supported, this FText should be populated with the reason.
	 * @return True if the pin can be dropped onto the node, false otherwise.
	 */
	virtual bool SupportsDropPinOnNode(UEdGraphNode* InTargetNode, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection, FText& OutErrorMessage) const override;

	/**
	 * @brief Checks if the editor's visualization cache for this graph type is out of date.
	 * @param InVisualizationCacheID The cache ID from the editor to compare against the schema's current ID.
	 * @return True if the cache is out of date and needs refreshing, false otherwise.
	 */
	virtual bool IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const override;

	/** @return The current visualization cache ID managed by this schema. */
	virtual int32 GetCurrentVisualizationCacheID() const override;

	/** Invalidates the current visualization cache by incrementing its ID, forcing a refresh. */
	virtual void ForceVisualizationCacheClear() const override;

	void CreateSectionFromSelection(class UEdGraph* Graph) const;
	//~ End UEdGraphSchema Interface

private:
	//~ Private Static Members
	/** @brief Static counter used to invalidate and refresh graph visualization caches. Incremented by ForceVisualizationCacheClear. */
	static int32 CurrentCacheRefreshID;
	//~ End Private Static Members
};