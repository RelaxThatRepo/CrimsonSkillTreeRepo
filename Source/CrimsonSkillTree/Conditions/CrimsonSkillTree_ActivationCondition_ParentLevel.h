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
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Construction
	// =============================================================================================================
	UCrimsonSkillTree_ActivationCondition_ParentLevel();

	// ~UCrimsonSkillTree_ActivationConditionBase Interface
	// =============================================================================================================
	/**
	 * @brief Checks if the target parent node meets the required level.
	 * @param OwningNode The node that this condition belongs to.
	 * @return True if the parent is at the required level or higher.
	 */
	virtual bool IsConditionMet_Implementation(const UCrimsonSkillTree_Node* OwningNode) const override;

	/**
	 * @brief Simulates if this condition would remain met if the target parent's level was altered.
	 * @param OwningConditionNode The node that this condition belongs to.
	 * @param OwnerActorContext The actor who owns the skill tree.
	 * @param NodeWhoseBenefitsAreAltered The node whose level is being hypothetically changed.
	 * @param bBenefitsAreBeingLost True if simulating a level decrease.
	 * @return True if the condition would still be met after the simulated change.
	 */
	virtual bool IsConditionStillMetIfBenefitsAltered_Implementation(const UCrimsonSkillTree_Node* OwningConditionNode, AActor* OwnerActorContext, const UCrimsonSkillTree_Node* NodeWhoseBenefitsAreAltered, bool bBenefitsAreBeingLost) const override;

	/**
	 * @brief Binds to the target parent node's state change delegate to monitor for changes.
	 * @param InOwningNode The node that owns this condition instance.
	 */
	virtual void BeginMonitoring_Implementation(UCrimsonSkillTree_Node* InOwningNode) override;

	/**
	 * @brief Unbinds from the target parent node's state change delegate.
	 */
	virtual void EndMonitoring_Implementation() override;

	/**
	 * @brief Gets the descriptive text for UI tooltips.
	 * @return A formatted FText describing the condition.
	 */
	virtual FText GetTooltipDescription_Implementation() const override;

#if WITH_EDITOR
	// ~Editor-Only
	// =============================================================================================================
	/**
	 * @brief Gets the descriptive text for the editor graph.
	 * @return A formatted FText describing the condition for display in the editor.
	 */
	virtual FText GetEditorDescription_Implementation() const override;

	/**
	 * @brief Called when a property is changed in the editor.
	 * @param PropertyChangedEvent The event data for the property change.
	 */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief The GUID of the specific parent node this condition targets. This should be set in the editor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parent Level Condition")
	FGuid TargetParentNodeGuid;

	/** @brief The minimum level the target parent must be for this condition to be met. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parent Level Condition", meta = (ClampMin = "1", UIMin = "1"))
	int32 RequiredLevel;

protected:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/
	/**
	 * @brief Callback function for when the monitored parent node's state changes.
	 */
	UFUNCTION()
	void OnParentNodeStateChanged();

	/**
	 * @brief Helper function to find the target node this condition is observing.
	 * @param OwningNode The node that this condition belongs to.
	 * @return A const pointer to the target node, or nullptr if not found.
	 */
	const UCrimsonSkillTree_Node* GetTargetNode(const UCrimsonSkillTree_Node* OwningNode) const;
};
