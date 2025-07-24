#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CrimsonSkillTree_ActivationConditionBase.h"
#include "CrimsonSkillTree_ActivationCondition_AttributeRequirement.generated.h"

class UCrimsonSkillTree_Node;

/**
 * @enum ECrimsonAttributeComparisonType
 * @brief Defines the type of comparison to perform for an attribute-based condition.
 */
UENUM(BlueprintType)
enum class ECrimsonAttributeComparisonType : uint8
{
	GreaterThan,
	LessThan,
	EqualTo,
	NotEqualTo,
	GreaterThanOrEqualTo,
	LessThanOrEqualTo
};

/**
 * @class UCrimsonSkillTree_ActivationCondition_AttributeRequirement
 * @brief An activation condition that requires a specific Gameplay Attribute to meet a defined threshold.
 */
UCLASS(Blueprintable, Meta = (DisplayName = "Attribute Requirement Condition"))
class CRIMSONSKILLTREE_API UCrimsonSkillTree_ActivationCondition_AttributeRequirement : public UCrimsonSkillTree_ActivationConditionBase
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Construction
	// =============================================================================================================
	UCrimsonSkillTree_ActivationCondition_AttributeRequirement();

	// ~UCrimsonSkillTree_ActivationConditionBase Interface
	// =============================================================================================================
	/**
	 * @brief Binds to the required attribute's change delegate to monitor for changes.
	 * @param OwningNode The node that owns this condition instance.
	 */
	virtual void BeginMonitoring_Implementation(UCrimsonSkillTree_Node* OwningNode) override;

	/**
	 * @brief Unbinds from the attribute's change delegate.
	 */
	virtual void EndMonitoring_Implementation() override;

	/**
	 * @brief Checks if the required attribute meets the specified value comparison.
	 * @param OwningNode The node that this condition belongs to.
	 * @return True if the attribute value satisfies the comparison.
	 */
	virtual bool IsConditionMet_Implementation(const UCrimsonSkillTree_Node* OwningNode) const override;

	/**
	 * @brief Simulates if this condition would remain met if another node's attribute modifications were altered.
	 * @param OwningConditionNode The node that this condition belongs to.
	 * @param OwnerActorContext The actor who owns the skill tree.
	 * @param NodeWhoseBenefitsAreAltered The node whose attribute effects are being hypothetically changed.
	 * @param bBenefitsAreBeingLost True if simulating the removal of an effect.
	 * @return True if the condition would still be met after the simulated change.
	 */
	virtual bool IsConditionStillMetIfBenefitsAltered_Implementation(const UCrimsonSkillTree_Node* OwningConditionNode, AActor* OwnerActorContext, const UCrimsonSkillTree_Node* NodeWhoseBenefitsAreAltered, bool bBenefitsAreBeingLost) const override;

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
#endif

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief The Gameplay Attribute that this condition monitors. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition Properties")
	FGameplayAttribute RequiredAttribute;

	/** @brief The threshold value the attribute must meet. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition Properties", meta = (UIMin = "0.0"))
	float RequiredValue;

	/** @brief Defines how the attribute's current value is compared against RequiredValue. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition Properties")
	ECrimsonAttributeComparisonType ComparisonType;

protected:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/
	/**
	 * @brief Internal callback triggered when the monitored attribute's value changes.
	 * @param Data Structure containing information about the attribute change.
	 */
	virtual void OnMonitoredAttributeChanged(const FOnAttributeChangeData& Data);

	/**
	 * @brief Performs the actual comparison between two float values based on the ComparisonType.
	 * @param ValueA The first value (typically the current attribute value).
	 * @param ValueB The second value (typically the required threshold).
	 * @return True if the comparison is satisfied.
	 */
	bool PerformComparison(float ValueA, float ValueB) const;

private:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief Cached pointer to the owner's AbilitySystemComponent for efficiency. */
	UPROPERTY(Transient)
	TWeakObjectPtr<UAbilitySystemComponent> CachedAbilitySystemComponent;

	/** @brief Handle for the delegate bound to attribute value changes. */
	FDelegateHandle AttributeChangeDelegateHandle;
};
