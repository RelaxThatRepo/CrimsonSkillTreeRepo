// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTree_AutoLayoutStrategy.h"
#include "CrimsonSkillTree_TreeLayoutStrategy.generated.h"
/**
 * 
 */
UCLASS()
class CRIMSONSKILLTREEEDITOR_API UCrimsonSkillTree_TreeLayoutStrategy : public UCrimsonSkillTree_AutoLayoutStrategy
{
	GENERATED_BODY()
public:
	UCrimsonSkillTree_TreeLayoutStrategy();
	virtual ~UCrimsonSkillTree_TreeLayoutStrategy();

	virtual void Layout(UEdGraph* EdGraph) override;

protected:
	void InitPass(UCrimsonSkillTree_Node* RootNode, const FVector2D& Anchor);
	bool ResolveConflictPass(UCrimsonSkillTree_Node* Node);

	bool ResolveConflict(UCrimsonSkillTree_Node* LRoot, UCrimsonSkillTree_Node* RRoot);

	void GetLeftContour(UCrimsonSkillTree_Node* RootNode, int32 Level, TArray<UCrimsonSkillTree_GraphNode*>& Contour);
	void GetRightContour(UCrimsonSkillTree_Node* RootNode, int32 Level, TArray<UCrimsonSkillTree_GraphNode*>& Contour);
	
	void ShiftSubTree(UCrimsonSkillTree_Node* RootNode, const FVector2D& Offset);

	void UpdateParentNodePosition(UCrimsonSkillTree_Node* RootNode);
};

