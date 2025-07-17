// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTree_NodeEventBase.h"
#include "GameplayAbilitySpecHandle.h"     // For FGameplayAbilitySpecHandle
#include "Abilities/GameplayAbility.h"     // For UGameplayAbility
#include "CrimsonSkillTree_NodeEvent_GrantGameplayAbility.generated.h"

class UAbilitySystemComponent;

/**
 * @class UCrimsonSkillTree_NodeEvent_GrantGameplayAbility
 * @brief A skill node event that grants a specified GameplayAbility to the owner.
 * This event handles the granting and removal of a gameplay ability, associating it with an optional input ID.
 * The level of the granted ability can also be tied to the skill node's level.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Grant Gameplay Ability Event"))
class CRIMSONSKILLTREE_API UCrimsonSkillTree_NodeEvent_GrantGameplayAbility : public UCrimsonSkillTree_NodeEventBase
{
	GENERATED_BODY()

public:
	//~ Properties
	/** @brief The GameplayAbility class to be granted when this event is triggered. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Ability")
	TSubclassOf<UGameplayAbility> AbilityToGrant;

	/** * @brief The Input ID to assign to the granted ability. 
	 * Use -1 for no specific input binding (e.g., for passive abilities or abilities triggered by other gameplay events).
	 * Otherwise, this should correspond to an input ID defined in your project's input settings (often an enum value).
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Ability", meta = (Tooltip = "InputID for this ability. -1 for none, otherwise map to project's input enum."))
	int32 InputID;
	//~ End Properties

	/**
	 * @brief Default constructor.
	 */
	UCrimsonSkillTree_NodeEvent_GrantGameplayAbility();

protected:
	//~ UCrimsonSkillTree_NodeEventBase Interface
	virtual void OnLevelUp_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel) override;
	virtual void OnLevelDown_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel) override;
	virtual void OnNodeReset_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 PreviousLevel) override;
	virtual FText GetTooltipDescription_Implementation(const UCrimsonSkillTree_Node* PotentialEmitterNode) const override;
	//~ End UCrimsonSkillTree_NodeEventBase Interface
	
	//~ Protected Helper Methods
	/**
	 * @brief Grants the configured GameplayAbility to the owner's AbilitySystemComponent or updates its level if already granted by this event.
	 * Manages the GrantedAbilityHandle.
	 * @param ASC The AbilitySystemComponent to grant the ability to.
	 * @param AbilityLevelToGrant The level at which to grant the ability (typically the skill node's current level).
	 */
	virtual void GrantOrUpdateAbility(UAbilitySystemComponent* ASC, int32 AbilityLevelToGrant);

	/**
	 * @brief Removes the GameplayAbility previously granted by this event from the owner's AbilitySystemComponent.
	 * Uses and invalidates the GrantedAbilityHandle.
	 * @param ASC The AbilitySystemComponent from which to remove the ability.
	 */
	virtual void RemoveAbility(UAbilitySystemComponent* ASC);
	//~ End Protected Helper Methods

	//~ Protected Properties
	/** @brief Handle to the currently granted gameplay ability spec. Used to update or remove the ability. Marked Transient as it's runtime state. */
	UPROPERTY(Transient)
	FGameplayAbilitySpecHandle GrantedAbilityHandle;
	//~ End Protected Properties
};