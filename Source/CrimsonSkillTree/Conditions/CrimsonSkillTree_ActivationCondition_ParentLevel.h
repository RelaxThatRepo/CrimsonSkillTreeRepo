// Copyright Crimson Sword Studio, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTree_ActivationConditionBase.h"
#include "CrimsonSkillTree_ActivationCondition_ParentLevel.generated.h"

/**
 * @class UCrimsonSkillTree_ActivationCondition_ParentLevel
 * @brief An activation condition that requires a specific parent node to be at or above a certain level.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Parent Node Level Condition"))
class CRIMSONSKILLTREE_API UCrimsonSkillTree_ActivationCondition_ParentLevel : public UCrimsonSkillTree_ActivationConditionBase
{
	GENERATED_BODY()

public:
	UCrimsonSkillTree_ActivationCondition_ParentLevel();

	//~ UCrimsonSkillTree_ActivationConditionBase Interface
	virtual bool IsConditionMet_Implementation(const UCrimsonSkillTree_Node* OwningNode) const override;
	virtual bool IsConditionStillMetIfBenefitsAltered_Implementation(const UCrimsonSkillTree_Node* OwningConditionNode, AActor* OwnerActorContext, const UCrimsonSkillTree_Node* NodeWhoseBenefitsAreAltered, bool bBenefitsAreBeingLost) const override;
	virtual void BeginMonitoring_Implementation(UCrimsonSkillTree_Node* InOwningNode) override;
	virtual void EndMonitoring_Implementation() override;
	//~ End UCrimsonSkillTree_ActivationConditionBase Interface

	//~ Editor-Only
#if WITH_EDITOR
	virtual FText GetEditorDescription_Implementation() const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	//~ End Editor-Only

	virtual FText GetTooltipDescription_Implementation() const override;

public:
	//~ Properties
	// The GUID of the specific parent node this condition targets. This should be set in the editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parent Level Condition")
	FGuid TargetParentNodeGuid;

	// The minimum level the target parent must be for this condition to be met.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parent Level Condition", meta = (ClampMin = "1", UIMin = "1"))
	int32 RequiredLevel;
	//~ End Properties

protected:
	/**
	 * @brief Finds the specific parent node this condition is targeting.
	 * @param OwningNode The node that this condition belongs to.
	 * @return A pointer to the target parent node, or nullptr if not found.
	 */
	const UCrimsonSkillTree_Node* GetTargetParentNode(const UCrimsonSkillTree_Node* OwningNode) const;

	/**
	 * @brief Centralized function to broadcast UI messages when a condition fails.
	 * @param FailureReason The specific text explaining why the condition failed.
	 * @param DependencyNode The node that is the source of the failure (the parent node in this case).
	 */
	void BroadcastFailureMessage(const FText& FailureReason, const UCrimsonSkillTree_Node* DependencyNode) const;

	/**
	 * @brief Callback function for when the monitored parent node's state changes.
	 */
	UFUNCTION()
	void OnParentNodeStateChanged();
};
