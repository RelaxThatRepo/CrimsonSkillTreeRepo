#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTreeWidget_LineDrawingPolicyBase.h"
#include "SkillTreeLineDrawingPolicy_StraightLine.generated.h"

/**
 * @class USkillTreeLineDrawingPolicy_StraightLine
 * @brief A simple line drawing policy that draws a direct straight line between node connection points.
 * @details This class relies entirely on the base class's default implementation of CreateVisualConnection.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Straight Line Drawer"))
class CRIMSONSKILLTREE_API USkillTreeLineDrawingPolicy_StraightLine : public UCrimsonSkillTreeWidget_LineDrawingPolicyBase
{
	GENERATED_BODY()
};
