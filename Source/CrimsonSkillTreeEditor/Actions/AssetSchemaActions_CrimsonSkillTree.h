// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CrimsonSkillTreeEditor/Nodes/CrimsonSkillTree_GraphNode.h" // Original include
#include "CrimsonSkillTreeEditor/Nodes/CrimsonSkillTree_GraphEdge.h" // Original include
#include "CrimsonSkillTreeEditor/Nodes/CrimsonSkillTree_GraphNode_Visual.h"

#include "EdGraph/EdGraphSchema.h" // Added for FEdGraphSchemaAction, assuming this is standard and was implicitly available or is needed for clarity
#include "AssetSchemaActions_CrimsonSkillTree.generated.h"


class UCrimsonSkillTree_AutoLayoutStrategy; // Original forward declaration

/**
 * @struct FAssetSchemaAction_CrimsonSkillTree_NewNode
 * @brief Represents an action in the graph editor context menu for creating a new skill tree node.
 * This action, when performed, creates and configures a new UCrimsonSkillTree_GraphNode.
 */
USTRUCT()
struct CRIMSONSKILLTREEEDITOR_API FAssetSchemaAction_CrimsonSkillTree_NewNode : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

public:
	/** @brief Default constructor. */
	FAssetSchemaAction_CrimsonSkillTree_NewNode(): NodeTemplate(nullptr) {}

	/**
	 * @brief Constructor to initialize the schema action with display information.
	 * @param InNodeCategory The category this action belongs to in the context menu.
	 * @param InMenuDesc The description text displayed in the context menu.
	 * @param InToolTip The tooltip text displayed when hovering over the action in the menu.
	 * @param InGrouping A value used for sorting actions within the same category.
	 */
	FAssetSchemaAction_CrimsonSkillTree_NewNode(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr) {}

	//~ FEdGraphSchemaAction Interface
	/**
	 * @brief Executes the action to create a new node in the graph.
	 * @param ParentGraph The graph where the new node will be added.
	 * @param FromPin Optional pin from which a connection might be initiated to the new node.
	 * @param Location The location in the graph editor where the new node should be placed.
	 * @param bSelectNewNode If true, the newly created node will be selected in the editor.
	 * @return A pointer to the newly created UEdGraphNode, or nullptr on failure.
	 */
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	
	/**
	 * @brief Adds any UObject FProperties to the reference collector.
	 * This is necessary for garbage collection to correctly track UObject references held by this struct.
	 * @param Collector The reference collector.
	 */
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	//~ End FEdGraphSchemaAction Interface

	/** @brief Template for the graph node to be created by this action. This is typically set up before the action is performed. */
	UPROPERTY()
	TObjectPtr<UCrimsonSkillTree_GraphNode> NodeTemplate;
};

/**
 * @struct FAssetSchemaAction_CrimsonSkillTree_NewEdge
 * @brief Represents an action in the graph editor context menu for creating a new skill tree edge.
 * This action, when performed, creates and configเพื่อs a new UCrimsonSkillTree_GraphEdge.
 * @note While edges are often created by direct pin-to-pin connections, this schema action could be used
 * for alternative edge creation methods or for specific types of edges if the system supported them.
 */
USTRUCT()
struct CRIMSONSKILLTREEEDITOR_API FAssetSchemaAction_CrimsonSkillTree_NewEdge : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

public:
	/** @brief Default constructor. */
	FAssetSchemaAction_CrimsonSkillTree_NewEdge(): NodeTemplate(nullptr){}

	/**
	 * @brief Constructor to initialize the schema action with display information.
	 * @param InNodeCategory The category this action belongs to in the context menu.
	 * @param InMenuDesc The description text displayed in the context menu.
	 * @param InToolTip The tooltip text displayed when hovering over the action in the menu.
	 * @param InGrouping A value used for sorting actions within the same category.
	 */
	FAssetSchemaAction_CrimsonSkillTree_NewEdge(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr) {}

	//~ FEdGraphSchemaAction Interface
	/**
	 * @brief Executes the action to create a new edge node in the graph.
	 * @param ParentGraph The graph where the new edge node will be added.
	 * @param FromPin Optional pin from which a connection might be initiated.
	 * @param Location The location in the graph editor where the new edge node (if it's a node itself) should be placed.
	 * @param bSelectNewNode If true, the newly created node will be selected in the editor.
	 * @return A pointer to the newly created UEdGraphNode (representing the edge), or nullptr on failure.
	 */
	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;
	
	/**
	 * @brief Adds any UObject FProperties to the reference collector.
	 * @param Collector The reference collector.
	 */
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	//~ End FEdGraphSchemaAction Interface

	/** @brief Template for the graph edge (as a node) to be created by this action. */
	UPROPERTY()
	TObjectPtr<UCrimsonSkillTree_GraphEdge> NodeTemplate;
};


USTRUCT()
struct CRIMSONSKILLTREEEDITOR_API FAssetSchemaAction_CrimsonSkillTree_NewVisualNode : public FEdGraphSchemaAction
{
	GENERATED_USTRUCT_BODY();

	FAssetSchemaAction_CrimsonSkillTree_NewVisualNode() : NodeTemplate(nullptr) {}

	FAssetSchemaAction_CrimsonSkillTree_NewVisualNode(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr){}

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override;

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	
	UPROPERTY()
	TObjectPtr<UCrimsonSkillTree_GraphNode_Visual> NodeTemplate;
};