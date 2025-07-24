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

	//~ SWidget interface
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	//~ End SWidget interface

protected:
	FVector2D GetNodeSize() const;
	void OnNameTextCommited(const FText& InText, ETextCommit::Type CommitInfo);
	FText GetNodeTitleForEditableText() const;

	FSlateColor GetBorderColor() const;
	
	/** Public wrapper to check if the node is selected exclusively. */
	bool IsNodeSelectedExclusively() const;

	/** Handler for when the user clicks on the resizer handle */
	FReply OnResizerMouseDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

private:
	UCrimsonSkillTree_GraphNode_Visual* VisualGraphNode = nullptr;

	/** A pointer to the title bar border widget, used for hit-testing. */
	TSharedPtr<SBorder> TitleBar;

	/** The user is currently resizing the node */
	bool bIsResizing = false;
};