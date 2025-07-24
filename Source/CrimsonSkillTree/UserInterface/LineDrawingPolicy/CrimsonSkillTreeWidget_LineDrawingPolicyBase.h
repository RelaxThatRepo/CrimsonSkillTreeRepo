#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CrimsonSkillTreeWidget_LineDrawingPolicyBase.generated.h"

class UCrimsonSkillTree;
class UCrimsonSkillTree_Node;
class UCrimsonSkillTreeWidget_Node;
class UCanvasPanel;
class UImage;

/**
 * @class UCrimsonSkillTreeWidget_LineDrawingPolicyBase
 * @brief Abstract base class defining a policy for drawing connection lines between skill tree node widgets.
 * @details This class provides the core interface and common properties for all line drawing strategies.
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class CRIMSONSKILLTREE_API UCrimsonSkillTreeWidget_LineDrawingPolicyBase : public UObject
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Construction
	// =============================================================================================================
	UCrimsonSkillTreeWidget_LineDrawingPolicyBase();

	// ~UObject Interface
	// =============================================================================================================
#if WITH_EDITOR
	/**
	 * @brief Called after a property has been changed in the editor.
	 * @param PropertyChangedEvent The event data.
	 */
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// ~Public Interface
	// =============================================================================================================
	/**
	 * @brief Main function to draw all connections for a given skill tree.
	 * @param CanvasPanel The UMG CanvasPanel to draw the lines onto.
	 * @param SkillTree The skill tree data asset containing the node relationships.
	 * @param NodeWidgetMap A map from skill tree node data to their corresponding UMG widgets.
	 * @param OutDrawnLineWidgets An output array that this function will populate with the created UImage widgets.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Tree Line Drawing")
	void DrawAllConnections(UCanvasPanel* CanvasPanel, UCrimsonSkillTree* SkillTree, const TMap<UCrimsonSkillTree_Node*, UCrimsonSkillTreeWidget_Node*>& NodeWidgetMap, TArray<UImage*>& OutDrawnLineWidgets);

protected:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Protected Drawing Logic
	// =============================================================================================================
	/**
	 * @brief Creates the visual representation of a connection between two node widgets.
	 * @details Subclasses override this to implement different line styles (e.g., straight, elbow, curved).
	 * @param CanvasPanel The canvas panel to draw onto.
	 * @param StartNodeWidget The widget for the parent node.
	 * @param EndNodeWidget The widget for the child node.
	 * @param InOutDrawnLineWidgets Array to add any created UImage line segments to.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Tree Line Drawing")
	void CreateVisualConnection(UCanvasPanel* CanvasPanel, UCrimsonSkillTreeWidget_Node* StartNodeWidget, UCrimsonSkillTreeWidget_Node* EndNodeWidget, TArray<UImage*>& InOutDrawnLineWidgets);

	/**
	 * @brief Helper function to create a single straight UImage widget representing a line segment.
	 * @param CanvasPanel The canvas panel to add the line segment to.
	 * @param SegmentStartPoint The starting point of the line segment in canvas space.
	 * @param SegmentEndPoint The ending point of the line segment in canvas space.
	 * @param ActualLineColor The color to apply to the line segment.
	 * @param ActualLineThickness The thickness to apply to the line segment.
	 * @return The created UImage widget, or nullptr on failure.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Line Drawing")
	virtual UImage* CreateLineSegmentWidget(UCanvasPanel* CanvasPanel, const FVector2D& SegmentStartPoint, const FVector2D& SegmentEndPoint, const FLinearColor& ActualLineColor, float ActualLineThickness);

	/**
	 * @brief Determines the connection point on a node widget from which an outgoing line should start.
	 * @param NodeWidget The node widget to get the point from.
	 * @return The connection point in local canvas space.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Tree Line Drawing")
	FVector2D GetOutputConnectionPoint(const UCrimsonSkillTreeWidget_Node* NodeWidget) const;

	/**
	 * @brief Determines the connection point on a node widget at which an incoming line should end.
	 * @param NodeWidget The node widget to get the point from.
	 * @return The connection point in local canvas space.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Tree Line Drawing")
	FVector2D GetInputConnectionPoint(const UCrimsonSkillTreeWidget_Node* NodeWidget) const;

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Native Implementations
	// =============================================================================================================
	virtual void DrawAllConnections_Implementation(UCanvasPanel* CanvasPanel, UCrimsonSkillTree* SkillTree, const TMap<UCrimsonSkillTree_Node*, UCrimsonSkillTreeWidget_Node*>& NodeWidgetMap, TArray<UImage*>& OutDrawnLineWidgets);
	virtual void CreateVisualConnection_Implementation(UCanvasPanel* CanvasPanel, UCrimsonSkillTreeWidget_Node* StartNodeWidget, UCrimsonSkillTreeWidget_Node* EndNodeWidget, TArray<UImage*>& InOutDrawnLineWidgets);
	virtual FVector2D GetOutputConnectionPoint_Implementation(const UCrimsonSkillTreeWidget_Node* NodeWidget) const;
	virtual FVector2D GetInputConnectionPoint_Implementation(const UCrimsonSkillTreeWidget_Node* NodeWidget) const;

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief The color of the line when the connection is active. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Line Style")
	FLinearColor ActiveLineColor;

	/** @brief The color of the line when the connection is inactive. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Line Style")
	FLinearColor InactiveLineColor;

	/** @brief The thickness of the connection line. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Line Style")
	float LineThickness;

	/** @brief The brush to use for drawing the line segments. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Line Style")
	FSlateBrush LineBrush;

	/** @brief The Z-Order for the line widgets, determining their render layer. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Line Style", meta = (ClampMin = "-100", ClampMax = "100"))
	int32 LineZOrder;
};
