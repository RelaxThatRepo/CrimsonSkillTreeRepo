#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTree_NodeEventBase.h"
#include "GameplayAbilitySpecHandle.h"
#include "Abilities/GameplayAbility.h"
#include "CrimsonSkillTree_NodeEvent_GrantGameplayAbility.generated.h"

class UAbilitySystemComponent;

/**
 * @class UCrimsonSkillTree_NodeEvent_GrantGameplayAbility
 * @brief A skill node event that grants a specified GameplayAbility to the owner.
 * @details This event handles the granting and removal of a gameplay ability, associating it with an optional input ID.
 * The level of the granted ability can also be tied to the skill node's level.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Grant Gameplay Ability Event"))
class CRIMSONSKILLTREE_API UCrimsonSkillTree_NodeEvent_GrantGameplayAbility : public UCrimsonSkillTree_NodeEventBase
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Construction
	// =============================================================================================================
	UCrimsonSkillTree_NodeEvent_GrantGameplayAbility();

protected:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~UCrimsonSkillTree_NodeEventBase Interface
	// =============================================================================================================
	/**
	 * @brief Grants or updates the ability when the node's level increases.
	 * @param EmitterNode The node whose level changed.
	 * @param NewLevel The new level.
	 * @param OldLevel The previous level.
	 */
	virtual void OnLevelUp_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel) override;

	/**
	 * @brief Removes or updates the ability when the node's level decreases.
	 * @param EmitterNode The node whose level changed.
	 * @param NewLevel The new level.
	 * @param OldLevel The previous level.
	 */
	virtual void OnLevelDown_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel) override;

	/**
	 * @brief Removes the ability completely when the node is reset.
	 * @param EmitterNode The node being reset.
	 * @param PreviousLevel The level of the node before it was reset.
	 */
	virtual void OnNodeReset_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 PreviousLevel) override;

	/**
	 * @brief Gets the descriptive text for UI tooltips.
	 * @param PotentialEmitterNode The node that would emit this event.
	 * @return A formatted FText describing the event's effect.
	 */
	virtual FText GetTooltipDescription_Implementation(const UCrimsonSkillTree_Node* PotentialEmitterNode) const override;

	// ~Protected Helpers
	// =============================================================================================================
	/**
	 * @brief Grants the configured GameplayAbility to the owner's ASC or updates its level if already granted.
	 * @param ASC The AbilitySystemComponent to grant the ability to.
	 * @param AbilityLevelToGrant The level at which to grant the ability.
	 */
	virtual void GrantOrUpdateAbility(UAbilitySystemComponent* ASC, int32 AbilityLevelToGrant);

	/**
	 * @brief Removes the GameplayAbility previously granted by this event from the owner's ASC.
	 * @param ASC The AbilitySystemComponent from which to remove the ability.
	 */
	virtual void RemoveAbility(UAbilitySystemComponent* ASC);

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief The GameplayAbility class to be granted when this event is triggered. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Ability")
	TSubclassOf<UGameplayAbility> AbilityToGrant;

	/**
	 * @brief The Input ID to assign to the granted ability.
	 * @details Use -1 for no specific input binding (e.g., for passive abilities).
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Ability", meta = (Tooltip = "InputID for this ability. -1 for none."))
	int32 InputID;

protected:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief Handle to the currently granted gameplay ability spec. Used to update or remove the ability. */
	UPROPERTY(Transient)
	FGameplayAbilitySpecHandle GrantedAbilityHandle;
};
