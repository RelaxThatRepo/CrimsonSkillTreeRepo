// Copyright Crimson Sword Studio, 2024. All Rights Reserved.

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
	UCrimsonSkillTree_ActivationCondition_AttributeRequirement();

	//~ UCrimsonSkillTree_ActivationConditionBase Interface
	virtual void BeginMonitoring_Implementation(UCrimsonSkillTree_Node* OwningNode) override;
	virtual void EndMonitoring_Implementation() override;
	virtual bool IsConditionMet_Implementation(const UCrimsonSkillTree_Node* OwningNode) const override;
	virtual bool IsConditionStillMetIfBenefitsAltered_Implementation(const UCrimsonSkillTree_Node* OwningConditionNode, AActor* OwnerActorContext, const UCrimsonSkillTree_Node* NodeWhoseBenefitsAreAltered, bool bBenefitsAreBeingLost) const override;
	//~ End UCrimsonSkillTree_ActivationConditionBase Interface

	//~ Editor-Only
#if WITH_EDITOR
	virtual FText GetEditorDescription_Implementation() const override;
#endif
	//~ End Editor-Only

	virtual FText GetTooltipDescription_Implementation() const override;
	
public:
	//~ Properties
	// The Gameplay Attribute that this condition monitors.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition Properties")
	FGameplayAttribute RequiredAttribute;

	// The threshold value the attribute must meet.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Condition Properties", meta = (UIMin = "0.0"))
	float RequiredValue;

	// Defines how the attribute's current value is compared against RequiredValue.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition Properties")
	ECrimsonAttributeComparisonType ComparisonType;
	//~ End Properties

protected:
	/**
	 * @brief Internal callback triggered when the monitored attribute's value changes.
	 * @param Data Structure containing information about the attribute change.
	 */
	virtual void OnMonitoredAttributeChanged(const FOnAttributeChangeData& Data);

	/**
	 * @brief Centralized function to broadcast UI messages when the condition fails a direct check.
	 * @param FailureReason The specific text explaining why the condition failed.
	 */
	void BroadcastFailureMessage(const FText& FailureReason) const;

	/**
	 * @brief Performs the actual comparison between two float values based on the ComparisonType.
	 * @param ValueA The first value (typically the current attribute value).
	 * @param ValueB The second value (typically the required threshold).
	 * @return True if the comparison is satisfied.
	 */
	bool PerformComparison(float ValueA, float ValueB) const;
	
private:
	//~ Private Properties
	// Cached pointer to the owner's AbilitySystemComponent for efficiency.
	UPROPERTY(Transient)
	TWeakObjectPtr<UAbilitySystemComponent> CachedAbilitySystemComponent;
	//~ End Private Properties

	//~ Private Members
	// Handle for the delegate bound to attribute value changes.
	FDelegateHandle AttributeChangeDelegateHandle;
	//~ End Private Members
};
