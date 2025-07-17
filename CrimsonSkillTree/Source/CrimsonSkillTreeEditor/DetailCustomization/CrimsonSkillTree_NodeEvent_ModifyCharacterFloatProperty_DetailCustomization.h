// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "CrimsonSkillTree/Events/CrimsonSkillTree_NodeEvent_ModifyCharacterFloatProperty.h"


// Helper struct for SComboBox items
struct FPropertyOption
{
	FName PropertyName;
	FText DisplayName; // Could be PropertyName.ToString() or a more friendly name

	FPropertyOption(FName InName, FText InDisplay) : PropertyName(InName), DisplayName(InDisplay) {}

	// For SComboBox comparison
	bool operator==(const FPropertyOption& Other) const { return PropertyName == Other.PropertyName; }
};

class FCrimsonSkillTree_NodeEvent_ModifyCharacterFloatProperty_DetailCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	void OnPropertyOwnerScopeChanged();
	void PopulateFloatPropertyOptions();

	TSharedRef<SWidget> OnGeneratePropertyOptionWidget(TSharedPtr<FPropertyOption> InOption);
	void OnPropertySelectionChanged(TSharedPtr<FPropertyOption> NewSelection, ESelectInfo::Type SelectInfo);
	FText GetCurrentPropertyNameText() const;


	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
	UCrimsonSkillTree_NodeEvent_ModifyCharacterFloatProperty* TargetEventInstance = nullptr; // The event instance being edited

	TSharedPtr<IPropertyHandle> PropertyOwnerScopeHandle;
	TSharedPtr<IPropertyHandle> TargetPropertyNameHandle;

	TArray<TSharedPtr<FPropertyOption>> FloatPropertyOptionsList;
	TSharedPtr<FPropertyOption> CurrentSelectedPropertyOption;
	TSharedPtr<SComboBox<TSharedPtr<FPropertyOption>>> PropertyComboBox;
    
	IDetailLayoutBuilder* CachedDetailBuilder = nullptr;
};