// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

/**
 * 
 */
class CRIMSONSKILLTREEEDITOR_API FCrimsonSkillTree_EditorStyle
{
public:
	static void Initialize();
	static void Shutdown();
	static const ISlateStyle& Get(); // Provides access to the style set
	static const FName& GetStyleSetName();

private:
	static TSharedPtr<FSlateStyleSet> StyleSet;
};
