// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"    // For FActiveGameplayEffectHandle
#include "AttributeSet.h"                  // For FGameplayAttribute
#include "CrimsonSkillTree_NodeEventBase.h"
#include "GameplayEffect.h"                // For UGameplayEffect, FGameplayModifierInfo
#include "GameplayTagContainer.h"          // For FGameplayTagContainer, FGameplayTag
#include "CrimsonSkillTree_NodeEvent_ApplyGameplayEffect.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;
struct FGameplayEffectSpec; // Forward declare for use in static method parameter

/**
 * @struct FSkillTreeSimulatedAttributePayload
 * @brief Represents a simulated change to a gameplay attribute, typically used by node events for predictive calculations.
 * This payload helps in determining the net effect of an event on an attribute without actually applying it.
 */
USTRUCT(BlueprintType)
struct FSkillTreeSimulatedAttributePayload // Example Payload Struct
{
	GENERATED_BODY()

	/** @brief The gameplay attribute that is affected by the simulation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree|Simulation")
	FGameplayAttribute Attribute;

	/** * @brief The net additive change to the attribute. 
	 * For example, if an event grants +10 to an attribute and is being simulated for reversal, this value would be -10.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree|Simulation")
	float NetAdditiveChange = 0.0f;
	
	// Future extensions could include other types of changes:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree|Simulation")
	// float NetMultiplicativeChange = 1.0f; 
};

/**
 * @class UCrimsonSkillTree_NodeEvent_ApplyGameplayEffect
 * @brief A skill node event that applies a specified GameplayEffect to the owner.
 * This event handles the application, updating (based on level changes), and removal of a GameplayEffect.
 * It also supports simulation of its impact on attributes for predictive skill tree operations.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Apply Gameplay Effect Event"))
class CRIMSONSKILLTREE_API UCrimsonSkillTree_NodeEvent_ApplyGameplayEffect : public UCrimsonSkillTree_NodeEventBase
{
	GENERATED_BODY()

public:
	//~ Static Utility Methods
	/**
	 * @brief Evaluates the magnitude of a gameplay effect modifier at a given level.
	 * Handles ScalableFloat and SetByCaller magnitude calculation types.
	 * @param Modifier The modifier info whose magnitude is to be evaluated.
	 * @param Level The level at which to evaluate the magnitude (e.g., skill node level or effect level).
	 * @param Spec Optional GameplayEffectSpec, required for SetByCaller magnitudes if the tag is not directly on the modifier.
	 * @param SBCMagnitudeTag The tag for SetByCaller magnitudes, used if Modifier.ModifierMagnitude is SetByCaller.
	 * @return The evaluated magnitude as a float.
	 */
	static float EvaluateModifierMagnitudeAtLevel(const FGameplayModifierInfo& Modifier, float Level,
	                                              const FGameplayEffectSpec* Spec = nullptr, const FGameplayTag& SBCMagnitudeTag = FGameplayTag());
	//~ End Static Utility Methods

	//~ Properties
	/** @brief The GameplayEffect class to apply when this event is triggered. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect")
	TSubclassOf<UGameplayEffect> GameplayEffectToApply;

	/** * @brief If true, the skill node's current level will be passed as the level of the GameplayEffect.
	 * The GameplayEffect itself must be configured to scale with its level for this to have a meaningful impact.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect")
	bool bSetGameplayEffectLevel;

	/** * @brief If not setting GE level directly, this tag can be used with SetByCaller on the GameplayEffect.
	 * The skill node's current level will be set as the magnitude for this tag.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect", meta = (EditCondition = "!bSetGameplayEffectLevel", EditConditionHides))
	FGameplayTag SetByCallerMagnitudeTag;

    /** * @brief If bSetGameplayEffectLevel is false AND SetByCallerMagnitudeTag is valid, this value is used for the SetByCaller magnitude.
     * This is typically used if the SetByCaller magnitude is fixed rather than scaling with the node's level directly through the tag.
     * If SetByCallerMagnitudeTag is meant to use the node's current level, this float value might be redundant or used as a multiplier.
     * The primary mechanism when !bSetGameplayEffectLevel is to use SetByCallerMagnitudeTag with the node's current level as the value.
     * This float provides an alternative fixed value if the SBC tag is present but a dynamic level value isn't desired for it.
     */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect", meta = (EditCondition = "!bSetGameplayEffectLevel", EditConditionHides))
	float SetByCallerMagnitude;

	/** * @brief The primary gameplay attribute this effect modifies. 
	 * This is used for generating a dynamic tooltip description of the event's effect.
	 * For accurate dynamic descriptions, this attribute should be set.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect|Description", meta = (Tooltip="The primary attribute this effect modifies, for tooltip generation."))
	FGameplayAttribute TargetAttributeForDescription;
	//~ End Properties

	/**
	 * @brief Default constructor.
	 */
	UCrimsonSkillTree_NodeEvent_ApplyGameplayEffect();
	
protected:
	//~ UCrimsonSkillTree_NodeEventBase Interface
	virtual void OnLevelUp_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel) override;
	virtual void OnLevelDown_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel) override;
	virtual void OnNodeReset_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 PreviousLevel) override;
	virtual void PopulateSimulationData_Implementation(const UCrimsonSkillTree_Node* EventContextNode, int32 OriginalEffectiveLevel, bool bSimulatingReversal, FSimulationDataCollector& OutDataCollector) const override;
	virtual FText GetTooltipDescription_Implementation(const UCrimsonSkillTree_Node* PotentialEmitterNode) const override;
	//~ End UCrimsonSkillTree_NodeEventBase Interface
	
	//~ Protected Helper Methods
	/**
	 * @brief Applies the configured GameplayEffect to the owner's AbilitySystemComponent or updates it if already active.
	 * Manages the ActiveEffectHandle.
	 * @param ASC The AbilitySystemComponent to apply the effect to.
	 * @param EffectLevel The level at which to apply/update the effect (e.g., skill node's current level).
	 * @param SourceTags Optional tags to add to the effect context, typically from the emitter node.
	 */
	virtual void ApplyOrUpdateEffect(UAbilitySystemComponent* ASC, int32 EffectLevel, const FGameplayTagContainer* SourceTags = nullptr);

	/**
	 * @brief Removes the currently active GameplayEffect instance from the owner's AbilitySystemComponent.
	 * Uses and invalidates the ActiveEffectHandle.
	 * @param ASC The AbilitySystemComponent from which to remove the effect.
	 */
	virtual void RemoveEffect(UAbilitySystemComponent* ASC);
	
	/**
	 * @brief Calculates the total additive magnitude for a specific attribute from a given GameplayEffect class,
	 * considering the effect's level and SetByCaller configurations of this event.
	 * @param EffectClass The GameplayEffect class to evaluate.
	 * @param TargetAttribute The specific attribute whose total additive modification is being calculated.
	 * @param EvaluationLevel The level at which to evaluate scalable floats within the GameplayEffect (e.g., node level).
	 * @param bEventSetsGELvel True if this event sets the GameplayEffect's level using EvaluationLevel.
	 * @param EventSetByCallerTag The SetByCaller tag configured on this event.
	 * @param EventSetByCallerValueForNodeLevel The value to use for SetByCaller if derived from the node's level (used when !bEventSetsGELvel).
	 * @param EventConfiguredSBCMagnitude The fixed SetByCaller magnitude configured on this event (used when bEventSetsGELvel is true but SBC tag is also used).
	 * @param ASCForSpec The AbilitySystemComponent used to create a temporary GameplayEffectSpec for evaluation.
	 * @param SourceObject The UObject to set as the source on the temporary GameplayEffectSpec's context.
	 * @return The total calculated additive magnitude for the TargetAttribute.
	 */
	float CalculateTotalAdditiveMagnitudeForAttribute(
		TSubclassOf<UGameplayEffect> EffectClass,
		FGameplayAttribute TargetAttribute, 
		float EvaluationLevel,
		bool bEventSetsGELvel, 
		const FGameplayTag& EventSetByCallerTag,
		float EventSetByCallerValueForNodeLevel, 
		float EventConfiguredSBCMagnitude,
		UAbilitySystemComponent* ASCForSpec, 
		const UObject* SourceObject) const;
	//~ End Protected Helper Methods

	//~ Protected Properties
	/** @brief Handle to the currently active gameplay effect instance. Used to update or remove the effect. Marked Transient as it's runtime state. */
	UPROPERTY(Transient)
	FActiveGameplayEffectHandle ActiveEffectHandle;
	//~ End Protected Properties
};