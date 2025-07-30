#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTree_ActivationConditionBase.h"
#include "CrimsonSkillTree_Condition_Composite.generated.h"

/**
 * @class UCrimsonSkillTree_Condition_CompositeBase
 * @brief An abstract base for conditions that contain other conditions.
 */
UCLASS(Abstract)
class CRIMSONSKILLTREE_API UCrimsonSkillTree_Condition_CompositeBase : public UCrimsonSkillTree_ActivationConditionBase
{
	GENERATED_BODY()

public:
	/** @brief The list of child conditions this operator will evaluate. */
	UPROPERTY(EditAnywhere, Instanced, Category = "Conditions")
	TArray<TObjectPtr<UCrimsonSkillTree_ActivationConditionBase>> ChildConditions;

	// This override is needed to pass the owning node down to child conditions.
	virtual void SetCachedOwningNode(UCrimsonSkillTree_Node* InOwningNode) override;


protected:
	// Propagate monitoring calls to all child conditions
	virtual void BeginMonitoring_Implementation(UCrimsonSkillTree_Node* InOwningNode) override;
	virtual void EndMonitoring_Implementation() override;
};


/**
 * @class UCrimsonSkillTree_Condition_AND
 * @brief A composite condition that returns true only if ALL of its child conditions are met.
 */
UCLASS(meta = (DisplayName = "AND Block"))
class CRIMSONSKILLTREE_API UCrimsonSkillTree_Condition_AND : public UCrimsonSkillTree_Condition_CompositeBase
{
	GENERATED_BODY()

protected:
	virtual bool IsConditionMet_Implementation(const UCrimsonSkillTree_Node* OwningNode) const override;
	virtual FText GetTooltipDescription_Implementation() const override;

	virtual bool IsConditionStillMetIfBenefitsAltered_Implementation(const UCrimsonSkillTree_Node* OwningConditionNode, AActor* OwnerActorContext, const UCrimsonSkillTree_Node* NodeWhoseBenefitsAreAltered, bool bBenefitsAreBeingLost) const override;
	
};


/**
 * @class UCrimsonSkillTree_Condition_OR
 * @brief A composite condition that returns true if ANY of its child conditions are met.
 */
UCLASS(meta = (DisplayName = "OR Block"))
class CRIMSONSKILLTREE_API UCrimsonSkillTree_Condition_OR : public UCrimsonSkillTree_Condition_CompositeBase
{
	GENERATED_BODY()

protected:
	virtual bool IsConditionMet_Implementation(const UCrimsonSkillTree_Node* OwningNode) const override;
	virtual FText GetTooltipDescription_Implementation() const override;

	virtual bool IsConditionStillMetIfBenefitsAltered_Implementation(const UCrimsonSkillTree_Node* OwningConditionNode, AActor* OwnerActorContext, const UCrimsonSkillTree_Node* NodeWhoseBenefitsAreAltered, bool bBenefitsAreBeingLost) const override;
	
};