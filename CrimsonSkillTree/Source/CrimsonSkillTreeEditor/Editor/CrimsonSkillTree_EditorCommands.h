// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class CRIMSONSKILLTREEEDITOR_API FCrimsonSkillTree_EditorCommands : public TCommands<FCrimsonSkillTree_EditorCommands>
{
public:
	/** Constructor */
	FCrimsonSkillTree_EditorCommands()
		: TCommands<FCrimsonSkillTree_EditorCommands>("CrimsonSkillTreeEditor", NSLOCTEXT("Contexts", "CrimsonSkillTreeEditor", "Crimson Skill Tree Editor"), NAME_None, FAppStyle::GetAppStyleSetName())
	{
	}
	
	TSharedPtr<FUICommandInfo> GraphSettings;
	TSharedPtr<FUICommandInfo> AutoArrange;
	TSharedPtr<FUICommandInfo> RebuildGraphButton;
	
	virtual void RegisterCommands() override;
};
