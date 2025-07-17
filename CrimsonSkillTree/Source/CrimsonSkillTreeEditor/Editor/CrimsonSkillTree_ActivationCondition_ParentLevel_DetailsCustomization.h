// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "UObject/Object.h"

// Forward declarations
class IDetailLayoutBuilder;
class UCrimsonSkillTree_Node;
class UCrimsonSkillActivationCondition_ParentLevel; // Your condition class

// Simple struct to hold display name and GUID for the SComboBox
struct FParentNodeOption
{
	FText DisplayName;
	FGuid NodeGuid;
	int32 ParentMaxLevel; // Store this for updating the RequiredLevel slider

	FParentNodeOption(const FText& InName, const FGuid& InGuid, int32 InMaxLevel)
		: DisplayName(InName), NodeGuid(InGuid), ParentMaxLevel(InMaxLevel) {}

	// For SComboBox comparison
	bool operator==(const FParentNodeOption& Other) const
	{
		return NodeGuid == Other.NodeGuid;
	}
};

class FCrimsonSkillTree_ActivationCondition_ParentLevel_DetailsCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	// SComboBox Callbacks
	TSharedRef<SWidget> OnGenerateParentNodeWidget(TSharedPtr<FParentNodeOption> InOption) const;
	void OnParentNodeSelectionChanged(TSharedPtr<FParentNodeOption> NewSelection, ESelectInfo::Type SelectInfo);
	FText GetCurrentParentNodeDisplayName() const;

	// Helper to populate parent options
	void PopulateParentNodeOptions();

	// Update RequiredLevel property constraints
	void UpdateRequiredLevelConstraints();

	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
	UCrimsonSkillTree_Node* OwningSkillNode = nullptr; // The node that owns this condition instance

	TArray<TSharedPtr<FParentNodeOption>> ParentNodeOptionsList;
	TSharedPtr<FParentNodeOption> CurrentSelectedParentOption;

	TSharedPtr<IPropertyHandle> TargetParentNodeGuidHandle;
	TSharedPtr<IPropertyHandle> RequiredLevelHandle;

	IDetailLayoutBuilder* CachedDetailBuilder = nullptr; // To force refresh
};
