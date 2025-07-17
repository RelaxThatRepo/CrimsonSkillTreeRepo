// Copyright Crimson Sword Studio, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTreeWidget_LineDrawingPolicyBase.h"
#include "SkillTreeLineDrawingPolicy_StraightLine.generated.h"

/**
 * @class USkillTreeLineDrawingPolicy_StraightLine
 * @brief A simple line drawing policy that draws a direct straight line between node connection points.
 * This class relies entirely on the base class's default implementation of CreateVisualConnection.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Straight Line Drawer"))
class CRIMSONSKILLTREE_API USkillTreeLineDrawingPolicy_StraightLine : public UCrimsonSkillTreeWidget_LineDrawingPolicyBase
{
	GENERATED_BODY()

	// No overrides are needed. This class exists to provide a distinct type in the editor
	// that uses the default straight-line behavior inherited from the base class.
};
