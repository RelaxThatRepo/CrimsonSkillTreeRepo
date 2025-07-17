// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphPin.h"
#include "CrimsonSkillTreeEditor/Editor/CrimsonSkillTree_GraphDragConnection.h"
#include "CrimsonSkillTreeEditor/Graph/CrimsonSkillTree_GraphColors.h"

/**
 * 
 */
class SCrimsonSkillTree_GraphPin: public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SCrimsonSkillTree_GraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		this->SetCursor(EMouseCursor::Default);

		bShowLabel = true;

		GraphPinObj = InPin;
		check(GraphPinObj != nullptr);

		const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
		check(Schema);
		
		SBorder::Construct(SBorder::FArguments()
			.BorderImage(this, &SCrimsonSkillTree_GraphPin::GetPinBorder)
			.BorderBackgroundColor(this, &SCrimsonSkillTree_GraphPin::GetPinColor)
			.OnMouseButtonDown(this, &SCrimsonSkillTree_GraphPin::OnPinMouseDown)
			.Cursor(this, &SCrimsonSkillTree_GraphPin::GetPinCursor)
			.Padding(FMargin(5.0f))
		);

		//SBorder::Construct(SBorder::FArguments()
		//	.BorderImage(FAppStyle::GetBrush("NoBorder")) // No visible border for the pin itself
		//	.BorderBackgroundColor(FSlateColor(FLinearColor::Transparent)) // Transparent background
		//	.OnMouseButtonDown(this, &SCrimsonSkillTree_GraphPin::OnPinMouseDown) // Crucial for dragging
		//	.Cursor(this, &SCrimsonSkillTree_GraphPin::GetPinCursor)
		//	.Padding(0.0f) // No internal padding for this SBorder
		//	.HAlign(HAlign_Fill) // Ensure its content fills
		//	.VAlign(VAlign_Fill)
		//	[
		//		// This SBox provides a minimal hit-testable area that fills the pin slot.
		//		// It's transparent by default.
		//		SNew(SBox)
		//		.HAlign(HAlign_Fill)
		//		.VAlign(VAlign_Fill)
		//		//.ToolTipText(this, &SGraphPin::GetTooltipText) // Keep tooltips for pin info
		//		// You could put a *very* subtle SImage here if you want a slight visual cue on hover,
		//		// but for making the red block the pin, this should be mostly invisible.
		//	]
		//);
		
	}

protected:
	virtual FSlateColor GetPinColor() const override
	{
		return CrimsonSkillTree_GraphColors::Pin::Default;
		//return FLinearColor::Transparent;
	}

	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override
	{
		//return SNullWidget::NullWidget;
		return SNew(STextBlock);
	}

	const FSlateBrush* GetPinBorder() const
	{
		return FAppStyle::GetBrush(TEXT("Graph.StateNode.Body"));
	}

	virtual TSharedRef<FDragDropOperation> SpawnPinDragEvent(const TSharedRef<class SGraphPanel>& InGraphPanel, const TArray< TSharedRef<SGraphPin> >& InStartingPins) override
	{
		FCrimsonSkillTree_GraphDragConnection::FDraggedPinTable PinHandles;
		PinHandles.Reserve(InStartingPins.Num());
		// since the graph can be refreshed and pins can be reconstructed/replaced 
		// behind the scenes, the DragDropOperation holds onto FGraphPinHandles 
		// instead of direct widgets/graph-pins
		for (const TSharedRef<SGraphPin>& PinWidget : InStartingPins)
		{
			PinHandles.Add(PinWidget->GetPinObj());
		}

		return FCrimsonSkillTree_GraphDragConnection::New(InGraphPanel, PinHandles);
	}
};
