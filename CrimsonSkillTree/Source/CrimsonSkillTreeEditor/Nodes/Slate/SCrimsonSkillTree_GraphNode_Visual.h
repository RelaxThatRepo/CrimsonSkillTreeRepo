// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class UCrimsonSkillTree_GraphNode_Visual;

class CRIMSONSKILLTREEEDITOR_API SCrimsonSkillTree_GraphNode_Visual : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SCrimsonSkillTree_GraphNode_Visual) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UCrimsonSkillTree_GraphNode_Visual* InNode);

	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

	//~ SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual void MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty = true) override;
	//~ End SGraphNode interface

protected:
	FVector2D GetNodeSize() const;
	void OnNameTextCommited(const FText& InText, ETextCommit::Type CommitInfo);
	FText GetNodeTitleForEditableText() const;
	/** Public wrapper to check if the node is selected exclusively. */
	bool IsNodeSelectedExclusively() const;
private:
	UCrimsonSkillTree_GraphNode_Visual* VisualGraphNode = nullptr;
};