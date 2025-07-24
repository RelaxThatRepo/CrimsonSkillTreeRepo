#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTree_ActivationConditionBase.h"
#include "CrimsonSkillTree/Nodes/Cost/CrimsonSkillTree_NodeCost.h"
#include "CrimsonSkillTree_ActivationCondition_ResourcePointsSpent.generated.h"

/**
 * @class UCrimsonSkillTree_ActivationCondition_ResourcePointsSpent
 * @brief An activation condition that requires a minimum number of points of a specific resource to have been spent across all skill trees.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Resource Points Spent Condition"))
class CRIMSONSKILLTREE_API UCrimsonSkillTree_ActivationCondition_ResourcePointsSpent : public UCrimsonSkillTree_ActivationConditionBase
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Construction
	// =============================================================================================================
	UCrimsonSkillTree_ActivationCondition_ResourcePointsSpent();

	// ~UCrimsonSkillTree_ActivationConditionBase Interface
	// =============================================================================================================
	/**
	 * @brief Checks if the required amount of a specific resource has been spent.
	 * @param OwningNode The node that this condition belongs to.
	 * @return True if the total spent amount is greater than or equal to the required amount.
	 */
	virtual bool IsConditionMet_Implementation(const UCrimsonSkillTree_Node* OwningNode) const override;

	/**
	 * @brief Simulates if this condition would remain met if another node's cost was refunded.
	 * @param OwningConditionNode The node that this condition belongs to.
	 * @param OwnerActorContext The actor who owns the skill tree.
	 * @param NodeWhoseBenefitsAreAltered The node whose cost is being hypothetically refunded.
	 * @param bBenefitsAreBeingLost True if simulating a refund/cost removal.
	 * @return True if the condition would still be met after the simulated refund.
	 */
	virtual bool IsConditionStillMetIfBenefitsAltered_Implementation(const UCrimsonSkillTree_Node* OwningConditionNode, AActor* OwnerActorContext, const UCrimsonSkillTree_Node* NodeWhoseBenefitsAreAltered, bool bBenefitsAreBeingLost) const override;

	/**
	 * @brief Gets the descriptive text for UI tooltips.
	 * @return A formatted FText describing the condition.
	 */
	virtual FText GetTooltipDescription_Implementation() const override;

#if WITH_EDITOR
	/**
	 * @brief Gets the descriptive text for the editor graph.
	 * @return A formatted FText describing the condition for display in the editor.
	 */
	virtual FText GetEditorDescription_Implementation() const override;
#endif

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/

	/**
	 * @brief The resource type to check the spent amount of.
	 * @details This defines which pool of points (e.g., Skill Points, Attribute Points) is being checked.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Condition")
	FNodeCostDefinition RequiredResource;

	/**
	 * @brief The minimum number of points of the specified resource that must be spent for this condition to be met.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Condition", meta = (ClampMin = "1", UIMin = "1"))
	int32 RequiredAmountSpent;
};
