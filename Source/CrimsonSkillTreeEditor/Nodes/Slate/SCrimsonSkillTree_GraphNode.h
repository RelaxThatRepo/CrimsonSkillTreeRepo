// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class UCrimsonSkillTree_GraphNode;
/**
 * 
 */
class CRIMSONSKILLTREEEDITOR_API SCrimsonSkillTree_GraphNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SCrimsonSkillTree_GraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UCrimsonSkillTree_GraphNode* InNode);

	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual bool IsNameReadOnly() const override;
	virtual void MoveTo(const FVector2f& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty = true) override;
	void OnNameTextCommited(const FText& InText, ETextCommit::Type CommitInfo);

	virtual FSlateColor GetBorderBackgroundColor() const;
	virtual FSlateColor GetBackgroundColor() const;
	virtual EVisibility GetDragOverMarkerVisibility() const;
	virtual const FSlateBrush* GetNameIcon() const;

	FSlateColor GetNodeOutlineColor() const;
	const FSlateBrush* GetNodeTextureBrush() const;


	TSharedPtr<SHorizontalBox> InputPinBox;
	TSharedPtr<SHorizontalBox> OutputPinBox;

	TSharedPtr<SVerticalBox> ServicesAreaBox; // For adding "services" widgets

	// For dynamically updating the texture SImage
	FSlateBrush NodeTextureBrush;

protected: // Or public
	FText GetNodeTitleForEditableText() const;
};
