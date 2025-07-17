// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTree_NodeEventBase.h"

#include "CrimsonSkillTree_NodeEvent_ModifyCharacterFloatProperty.generated.h"

class ACharacter;

/**
 *@brief Enum to represent which classes to display in the drop down section.
 * After adding here, you must add logic in "FCrimsonSkillTree_NodeEvent_ModifyCharacterFloatProperty_DetailCustomization::PopulateFloatPropertyOptions()"
 * and in "void UCrimsonSkillTree_NodeEvent_ModifyCharacterFloatProperty::ApplyModification(float DeltaTotalValue) const".
 * Follow the examples as already shown.
 */
// Enum to represent common target scopes
UENUM(BlueprintType)
enum class EPropertyOwnerScope : uint8
{
    SelfActor                   UMETA(DisplayName = "Owner Actor (Character)"),
    CharacterMovementComponent  UMETA(DisplayName = "Character Movement Component"),
    PlayerState                 UMETA(DisplayName = "Player State"),
    Controller                  UMETA(DisplayName = "Controller"),
    // Add more if needed, e.g.: specific attached components by class
};

/**
 * @class UCrimsonSkillTree_NodeEvent_ModifyCharacterFloatProperty
 * @brief A base skill node event designed to modify a specified float UPROPERTY on the owning ACharacter or its UCharacterMovementComponent.
 *
 * This event applies an additive change per skill level. When a skill node's level increases,
 * the configured value per level is added to the target property. When it decreases or the node
 * is reset, the corresponding value is subtracted.
 *
 * The target property is identified by its FName. This class uses reflection to find and modify
 * the property, so ensure the property name is correct and the property is a float.
 *
 * @note This event should typically be executed on the server if the modified property affects
 * gameplay mechanics and needs to be authoritative.
 */
UCLASS(Blueprintable, meta = (DisplayName = "Modify Character Float Property Base Event")) // Abstract because it needs TargetPropertyName set
class CRIMSONSKILLTREE_API UCrimsonSkillTree_NodeEvent_ModifyCharacterFloatProperty : public UCrimsonSkillTree_NodeEventBase
{
    GENERATED_BODY()

public:
    /**
     * @brief Default constructor.
     * Initializes default values for properties.
     */
    UCrimsonSkillTree_NodeEvent_ModifyCharacterFloatProperty();

    /**
     * @brief User selects the scope from which to pick the property
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Modification", meta = (Tooltip = "Where to look for the property to modify."))
    EPropertyOwnerScope PropertyOwnerScope;

    /**
     * @brief The name of the float UPROPERTY to modify on the Character or its CharacterMovementComponent.
     * This name must exactly match the UPROPERTY name in the target class.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Modification", meta = (Tooltip = "Name of the float UPROPERTY to modify (e.g., MaxWalkSpeed, Health)."))
    FName TargetPropertyName;

    /**
     * @brief The amount to add to the target property for EACH level of the skill node.
     * For example, if this is 10.0 and the skill node gains 2 levels, a total of 20.0 will be added.
     * When leveling down or resetting, this amount per level will be subtracted.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property Modification", meta = (Tooltip = "Value to add per skill level. Can be negative for decreases."))
    float AdditiveValuePerLevel;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Property Modification")
    FText PropertyUserFacingName;

protected:
    //~ Begin UCrimsonSkillNodeEventBase Interface
    virtual void OnLevelUp_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel) override;
    virtual void OnLevelDown_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel) override;
    virtual void OnNodeReset_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 PreviousLevel) override;
    //~ End UCrimsonSkillNodeEventBase Interface

    /**
     * @brief Helper function to find and apply the float property modification.
     * @param DeltaTotalValue The total change to apply to the property (can be positive or negative).
     * @note This function should only execute on the server if the property is gameplay-critical.
     * The authority check is expected within this function's implementation.
     */
    void ApplyModification(float DeltaTotalValue) const;
    
    virtual FText GetTooltipDescription_Implementation(const UCrimsonSkillTree_Node* PotentialEmitterNode) const override;
};


