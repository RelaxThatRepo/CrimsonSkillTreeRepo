#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTree_NodeEventBase.h"
#include "CrimsonSkillTree_NodeEvent_ModifyCharacterFloatProperty.generated.h"

class ACharacter;
class UCurveFloat;

/**
 * @enum EPropertyOwnerScope
 * @brief Defines the scope on an ACharacter from which to find a property to modify.
 */
UENUM(BlueprintType)
enum class EPropertyOwnerScope : uint8
{
	SelfActor UMETA(DisplayName = "Owner Actor (Character)"),
	CharacterMovementComponent UMETA(DisplayName = "Character Movement Component"),
	PlayerState UMETA(DisplayName = "Player State"),
	Controller UMETA(DisplayName = "Controller"),
};

/**
 * @class UCrimsonSkillTree_NodeEvent_ModifyCharacterFloatProperty
 * @brief A skill node event that modifies a specified float UPROPERTY on the owning ACharacter or one of its standard components.
 * @details This event applies an additive change based on a curve. When a skill node's level changes,
 * the delta between the curve value at the old and new level is applied to the target property.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Modify Character Float Property Event"))
class CRIMSONSKILLTREE_API UCrimsonSkillTree_NodeEvent_ModifyCharacterFloatProperty : public UCrimsonSkillTree_NodeEventBase
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Construction
	// =============================================================================================================
	UCrimsonSkillTree_NodeEvent_ModifyCharacterFloatProperty();

protected:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~UCrimsonSkillTree_NodeEventBase Interface
	// =============================================================================================================
	/**
	 * @brief Applies the value change when the node's level increases.
	 * @param EmitterNode The node whose level changed.
	 * @param NewLevel The new level.
	 * @param OldLevel The previous level.
	 */
	virtual void OnLevelUp_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel) override;

	/**
	 * @brief Applies the value change (reversal) when the node's level decreases.
	 * @param EmitterNode The node whose level changed.
	 * @param NewLevel The new level.
	 * @param OldLevel The previous level.
	 */
	virtual void OnLevelDown_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel) override;

	/**
	 * @brief Completely reverses the total value applied by the node when it is reset.
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
	 * @brief Helper function to find and apply the float property modification.
	 * @param DeltaTotalValue The total change to apply to the property (can be positive or negative).
	 */
	void ApplyModification(float DeltaTotalValue) const;

private:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/
	/**
	 * @brief Evaluates the ValueCurve at a given level to find the total value.
	 * @param Level The level to evaluate the curve at.
	 * @return The float value from the curve at the specified level, or 0.0 if the curve is invalid.
	 */
	float GetTotalValueForLevel(int32 Level) const;

	/**
	 * @brief Helper function to get the target object based on the PropertyOwnerScope.
	 * @return The UObject to modify, or nullptr if not found.
	 */
	UObject* GetTargetObject() const;

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief The scope on the owning actor where the target property resides. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Modification")
	EPropertyOwnerScope PropertyOwnerScope;

	/** @brief The name of the float UPROPERTY to modify. Must match the property name in the target class exactly. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Modification")
	FName TargetPropertyName;

	/** @brief A curve that dictates the total value to be applied at each level (X-axis: Level, Y-axis: Total Value). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Modification")
	TObjectPtr<UCurveFloat> ValueCurve;

	/** @brief The user-friendly name of the property being modified, for use in descriptions. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property Modification")
	FText PropertyUserFacingName;
};
