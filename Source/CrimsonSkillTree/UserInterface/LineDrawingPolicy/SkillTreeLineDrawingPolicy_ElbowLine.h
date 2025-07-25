#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTreeWidget_LineDrawingPolicyBase.h"
#include "SkillTreeLineDrawingPolicy_ElbowLine.generated.h"

/**
 * @class USkillTreeLineDrawingPolicy_ElbowLine
 * @brief A line drawing policy that draws connections as elbow lines (three segments: vertical, horizontal, vertical).
 */
UCLASS(Blueprintable, meta = (DisplayName = "Elbow Line Drawer"))
class CRIMSONSKILLTREE_API USkillTreeLineDrawingPolicy_ElbowLine : public UCrimsonSkillTreeWidget_LineDrawingPolicyBase
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Construction
	// =============================================================================================================
	USkillTreeLineDrawingPolicy_ElbowLine();

protected:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~UCrimsonSkillTreeWidget_LineDrawingPolicyBase Interface
	// =============================================================================================================
	/**
	 * @brief Overridden to draw the elbow line using three distinct segments.
	 * @param CanvasPanel The canvas panel to draw onto.
	 * @param StartNodeWidget The widget for the parent node.
	 * @param EndNodeWidget The widget for the child node.
	 * @param InOutDrawnLineWidgets Array to add any created UImage line segments to.
	 */
	virtual void CreateVisualConnection_Implementation(UCanvasPanel* CanvasPanel, UCrimsonSkillTreeWidget_Node* StartNodeWidget, UCrimsonSkillTreeWidget_Node* EndNodeWidget, TArray<UImage*>& InOutDrawnLineWidgets) override;

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief Defines how far vertically the line extends from the parent node before turning horizontal. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Elbow Line Style", meta = (ClampMin = "0"))
	float VerticalStepSize;

	/** @brief If true, the horizontal segment of the elbow line will be positioned at the vertical midpoint between the nodes. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Elbow Line Style")
	bool bCenterHorizontalSegment;
};
