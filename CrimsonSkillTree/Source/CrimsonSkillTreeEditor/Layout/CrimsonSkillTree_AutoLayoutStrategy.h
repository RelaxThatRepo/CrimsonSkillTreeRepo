// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CrimsonSkillTree_AutoLayoutStrategy.generated.h"

class UCrimsonSkillTree_Graph;
class UCrimsonSkillTree;
class UCrimsonSkillTree_Node;
class UCrimsonSkillTree_GraphNode;
/**
 * 
 */
UCLASS(Abstract)
class CRIMSONSKILLTREEEDITOR_API UCrimsonSkillTree_AutoLayoutStrategy: public UObject
{
	GENERATED_BODY()
public:
	UCrimsonSkillTree_AutoLayoutStrategy();
	virtual ~UCrimsonSkillTree_AutoLayoutStrategy();

	virtual void Layout(UEdGraph* G) {};

protected:
	int32 GetNodeWidth(UCrimsonSkillTree_GraphNode* EdNode);

	int32 GetNodeHeight(UCrimsonSkillTree_GraphNode* EdNode);

	FBox2D GetNodeBound(UEdGraphNode* EdNode);

	FBox2D GetActualBounds(UCrimsonSkillTree_Node* RootNode);

	virtual void RandomLayoutOneTree(UCrimsonSkillTree_Node* RootNode, const FBox2D& Bound);

public:
	UPROPERTY()
	TObjectPtr<class UCrimsonSkillTree_EditorSettings> Settings;
	
protected:
	UPROPERTY()
	TObjectPtr<UCrimsonSkillTree> Graph;
	
	UPROPERTY()
	TObjectPtr<UCrimsonSkillTree_Graph> EdGraph;
	
	int32 MaxIteration;
	int32 OptimalDistance;
};