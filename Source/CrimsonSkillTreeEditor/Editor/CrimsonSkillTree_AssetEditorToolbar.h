// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTree_AssetEditor.h"

/**
 * 
 */
class CRIMSONSKILLTREEEDITOR_API FCrimsonSkillTree_AssetEditorToolbar : public TSharedFromThis<FCrimsonSkillTree_AssetEditorToolbar>
{
public:
	FCrimsonSkillTree_AssetEditorToolbar(const TSharedPtr<FCrimsonSkillTree_AssetEditor>& InCrimsonSkillTreeEditor)
		: CrimsonSkillTreeEditor(InCrimsonSkillTreeEditor) {}

	void AddCrimsonSkillTreeToolbar(TSharedPtr<FExtender> Extender);

private:
	void FillCrimsonSkillTreeToolbar(FToolBarBuilder& ToolbarBuilder);

protected:
	/** Pointer back to the blueprint editor tool that owns us */
	TWeakPtr<FCrimsonSkillTree_AssetEditor> CrimsonSkillTreeEditor;
};
