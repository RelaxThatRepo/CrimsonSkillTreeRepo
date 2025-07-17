// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CrimsonSkillTree_EditorDevSettings.generated.h"

class UCrimsonSkillTree_SlateEdgeFactory;
class UCrimsonSkillTree_SlateNodeFactory;
/**
 * 
 */
UCLASS(config = Editor , defaultconfig, meta = (DisplayName = "Crimson Skill Tree Editor"))
class CRIMSONSKILLTREEEDITOR_API UCrimsonSkillTree_EditorDevSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UCrimsonSkillTree_EditorDevSettings();

	// The factory class to use for creating SGraphNode widgets for regular skill nodes.
	UPROPERTY(config, EditDefaultsOnly, Category = "Editor Appearance|Slate Factories", meta = (MustImplement = "/Script/CrimsonSkillTreeEditor.CrimsonSkillTree_SlateNodeFactory"))
	TSubclassOf<UCrimsonSkillTree_SlateNodeFactory> GraphNodeSlateFactoryClass;

	// The factory class to use for creating SGraphEdge widgets (or SGraphNode for edges).
	UPROPERTY(config, EditDefaultsOnly, Category = "Editor Appearance|Slate Factories", meta = (MustImplement = "/Script/CrimsonSkillTreeEditor.CrimsonSkillTree_SlateEdgeFactory"))
	TSubclassOf<UCrimsonSkillTree_SlateEdgeFactory> GraphEdgeSlateFactoryClass;
};
