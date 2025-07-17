// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class UCrimsonSkillTree_GraphNode;
/**
 * 
 */
class FCrimsonSkillTree_GraphNodeDetailsCustomization : public IDetailCustomization
{
public:
	// Called to create an instance of this customization
	static TSharedRef<IDetailCustomization> MakeInstance();

	// IDetailCustomization interface override
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	// Helper to get the first selected UCrimsonSkillTree_GraphNode
	static UCrimsonSkillTree_GraphNode* GetFirstSelectedGraphNode(const IDetailLayoutBuilder& DetailBuilder);

	// Optional: If you want to cache the selected objects (though DetailBuilder provides them)
	// TArray<TWeakObjectPtr<UObject>> SelectedObjects;
};
