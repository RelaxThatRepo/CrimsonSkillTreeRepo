#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "AttributeSet.h"
#include "CrimsonSkillTree_NodeEventBase.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "CrimsonSkillTree_NodeEvent_ApplyGameplayEffect.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;
struct FGameplayEffectSpec;

/**
 * @struct FSkillTreeSimulatedAttributePayload
 * @brief Represents a simulated change to a gameplay attribute, used by node events for predictive calculations.
 */
USTRUCT(BlueprintType)
struct FSkillTreeSimulatedAttributePayload
{
	GENERATED_BODY()

	/** @brief The gameplay attribute that is affected by the simulation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree|Simulation")
	FGameplayAttribute Attribute;

	/** @brief The net additive change to the attribute. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree|Simulation")
	float NetAdditiveChange = 0.0f;
};

/**
 * @class UCrimsonSkillTree_NodeEvent_ApplyGameplayEffect
 * @brief A skill node event that applies a specified GameplayEffect to the owner.
 * @details This event handles the application, updating, and removal of a GameplayEffect.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Apply Gameplay Effect Event"))
class CRIMSONSKILLTREE_API UCrimsonSkillTree_NodeEvent_ApplyGameplayEffect : public UCrimsonSkillTree_NodeEventBase
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Construction
	// =============================================================================================================
	UCrimsonSkillTree_NodeEvent_ApplyGameplayEffect();

	// ~Static Utility Methods
	// =============================================================================================================
	/**
	 * @brief Evaluates the magnitude of a gameplay effect modifier at a given level.
	 * @param Modifier The modifier info whose magnitude is to be evaluated.
	 * @param Level The level at which to evaluate the magnitude.
	 * @param Spec Optional GameplayEffectSpec, required for SetByCaller magnitudes.
	 * @param SBCMagnitudeTag The tag for SetByCaller magnitudes.
	 * @return The evaluated magnitude as a float.
	 */
	static float EvaluateModifierMagnitudeAtLevel(const FGameplayModifierInfo& Modifier, float Level, const FGameplayEffectSpec* Spec = nullptr, const FGameplayTag& SBCMagnitudeTag = FGameplayTag());

protected:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~UCrimsonSkillTree_NodeEventBase Interface
	// =============================================================================================================
	virtual void OnLevelUp_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel) override;
	virtual void OnLevelDown_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel) override;
	virtual void OnNodeReset_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 PreviousLevel) override;
	virtual void PopulateSimulationData_Implementation(const UCrimsonSkillTree_Node* EventContextNode, int32 OriginalEffectiveLevel, bool bSimulatingReversal, FSimulationDataCollector& OutDataCollector) const override;
	virtual FText GetTooltipDescription_Implementation(const UCrimsonSkillTree_Node* PotentialEmitterNode) const override;

	// ~Protected Helper Methods
	// =============================================================================================================
	/**
	 * @brief Applies the configured GameplayEffect to the owner's ASC or updates it if already active.
	 * @param ASC The AbilitySystemComponent to apply the effect to.
	 * @param EffectLevel The level at which to apply/update the effect.
	 * @param SourceTags Optional tags to add to the effect context.
	 */
	virtual void ApplyOrUpdateEffect(UAbilitySystemComponent* ASC, int32 EffectLevel, const FGameplayTagContainer* SourceTags = nullptr);

	/**
	 * @brief Removes the currently active GameplayEffect instance from the owner's ASC.
	 * @param ASC The AbilitySystemComponent from which to remove the effect.
	 */
	virtual void RemoveEffect(UAbilitySystemComponent* ASC);

	/**
	 * @brief Calculates the total additive magnitude for a specific attribute from a given GameplayEffect class.
	 * @param EffectClass The GameplayEffect class to evaluate.
	 * @param TargetAttribute The specific attribute whose total additive modification is being calculated.
	 * @param EvaluationLevel The level at which to evaluate scalable floats.
	 * @param bEventSetsGELvel True if this event sets the GameplayEffect's level.
	 * @param EventSetByCallerTag The SetByCaller tag configured on this event.
	 * @param EventSetByCallerValueForNodeLevel The value to use for SetByCaller if derived from the node's level.
	 * @param EventConfiguredSBCMagnitude The fixed SetByCaller magnitude configured on this event.
	 * @param ASCForSpec The AbilitySystemComponent used to create a temporary GameplayEffectSpec.
	 * @param SourceObject The UObject to set as the source on the temporary spec's context.
	 * @return The total calculated additive magnitude for the TargetAttribute.
	 */
	float CalculateTotalAdditiveMagnitudeForAttribute(TSubclassOf<UGameplayEffect> EffectClass, FGameplayAttribute TargetAttribute, float EvaluationLevel, bool bEventSetsGELvel, const FGameplayTag& EventSetByCallerTag, float EventSetByCallerValueForNodeLevel, float EventConfiguredSBCMagnitude, UAbilitySystemComponent* ASCForSpec, const UObject* SourceObject) const;

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief The GameplayEffect class to apply when this event is triggered. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect")
	TSubclassOf<UGameplayEffect> GameplayEffectToApply;

	/** @brief If true, the skill node's current level will be passed as the level of the GameplayEffect. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect")
	bool bSetGameplayEffectLevel;

	/** @brief If not setting GE level directly, this tag can be used with SetByCaller on the GameplayEffect. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect", meta = (EditCondition = "!bSetGameplayEffectLevel", EditConditionHides))
	FGameplayTag SetByCallerMagnitudeTag;

	/** @brief If bSetGameplayEffectLevel is false AND SetByCallerMagnitudeTag is valid, this value is used for the SetByCaller magnitude. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect", meta = (EditCondition = "!bSetGameplayEffectLevel", EditConditionHides))
	float SetByCallerMagnitude;

	/** @brief The primary gameplay attribute this effect modifies, used for generating a dynamic tooltip description. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effect", meta = (Tooltip = "The primary attribute this effect modifies, for tooltip generation."))
	FGameplayAttribute TargetAttributeForDescription;

protected:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief Handle to the currently active gameplay effect instance. Used to update or remove the effect. */
	UPROPERTY(Transient)
	FActiveGameplayEffectHandle ActiveEffectHandle;
};
