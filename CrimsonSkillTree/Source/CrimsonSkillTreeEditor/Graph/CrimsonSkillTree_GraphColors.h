#pragma once

#include "CoreMinimal.h"

namespace CrimsonSkillTree_GraphColors
{
	namespace NodeBody
	{
		constexpr FLinearColor Default(0.1f, 0.1f, 0.1f);
		constexpr FLinearColor Root(0.6f, 0.0f, 0.0f, 0.8f);
		constexpr  FLinearColor Error(0.0f, 0.0f, 0.0f);
		constexpr  FLinearColor NameText(1.0f, 1.0f, 1.0f);
	}

	namespace NodeBorder
	{
		constexpr FLinearColor Inactive(0.4f, 0.4f, 0.4f);
		constexpr FLinearColor Root(0.6f, 0.0f, 0.0f, 0.8f);
		constexpr FLinearColor Selected(0.0f, 0.6f, 0.0f);
		constexpr FLinearColor ActiveDebugging(1.0f, 1.0f, 0.0f);
		constexpr FLinearColor InactiveDebugging(0.4f, 0.4f, 0.0f);
		constexpr FLinearColor HighlightAbortRange0(0.0f, 0.22f, 0.4f);
		constexpr FLinearColor HighlightAbortRange1(0.0f, 0.4f, 0.22f);
		constexpr FLinearColor Disconnected(0.f, 0.f, 0.f);
		constexpr FLinearColor BrokenWithParent(1.f, 0.f, 1.f);
		constexpr FLinearColor QuickFind(0.f, 0.8f, 0.f);
	}

	namespace Pin
	{
		constexpr FLinearColor Diff(0.5f, 0.5f, 0.5f);
		constexpr FLinearColor Hover(0.0f, 0.7f, 0.0f);
		constexpr FLinearColor Default(0.0f, 0.0f, 0.0f);
		constexpr FLinearColor SingleNode(1.0f, 1.0f, 1.0f);
	}
	
	namespace Connection
	{
		constexpr FLinearColor Default(1.0f, 1.0f, 1.0f);
	}

	namespace Action
	{
		constexpr FLinearColor DragMarker(1.0f, 1.0f, 0.2f);
	}
}
