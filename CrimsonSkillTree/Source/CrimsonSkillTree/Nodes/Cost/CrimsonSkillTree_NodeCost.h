// Copyright Crimson Sword Studio, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Engine/DataTable.h"
#include "CrimsonSkillTree_NodeCost.generated.h"

#define LOCTEXT_NAMESPACE "CrimsonSkillTree"

/**
 * @enum ENodeCostSource
 * @brief Defines the source of a resource used for skill node costs.
 */
UENUM(BlueprintType)
enum class ENodeCostSource : uint8
{
    NoCost,
    ActorIntegerProperty,
    GameplayAttributeValue
};

/**
 * @struct FNodeCostDefinition
 * @brief Defines a specific type of resource cost (e.g., Skill Points, Gold).
 * This is a hashable struct, allowing it to be used as a key in TMaps.
 */
USTRUCT(BlueprintType)
struct FNodeCostDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost Definition")
    ENodeCostSource CostSource = ENodeCostSource::NoCost;

    // User-facing name for this resource (e.g., "Skill Points").
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost Definition")
    FText ResourceUserFacingName;

    // The name of an integer property on the owner actor to use as the resource pool.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost Definition", meta = (EditCondition = "CostSource == ENodeCostSource::ActorIntegerProperty"))
    FName ActorResourcePropertyName;

    // The gameplay attribute to use as the resource pool.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost Definition", meta = (EditCondition = "CostSource == ENodeCostSource::GameplayAttributeValue"))
    FGameplayAttribute ResourceGameplayAttribute;

    // Equality operator for TMap support.
    bool operator==(const FNodeCostDefinition& Other) const
    {
        if (CostSource != Other.CostSource) return false;
        switch (CostSource)
        {
        case ENodeCostSource::ActorIntegerProperty:
            return ActorResourcePropertyName == Other.ActorResourcePropertyName;
        case ENodeCostSource::GameplayAttributeValue:
            return ResourceGameplayAttribute == Other.ResourceGameplayAttribute;
        case ENodeCostSource::NoCost:
        default:
            return true;
        }
    }

	// Helper function to get a clean resource name, with fallbacks.
	FText GetResourceName() const
	{
		if (!ResourceUserFacingName.IsEmpty())
		{
			return ResourceUserFacingName;
		}
		if (CostSource == ENodeCostSource::ActorIntegerProperty && ActorResourcePropertyName != NAME_None)
		{
			return FText::FromName(ActorResourcePropertyName);
		}
		if (CostSource == ENodeCostSource::GameplayAttributeValue && ResourceGameplayAttribute.IsValid())
		{
			return FText::FromString(ResourceGameplayAttribute.GetName());
		}
		return LOCTEXT("UnknownResourceName", "Resource");
	}
};

// Hash function for TMap support.
inline uint32 GetTypeHash(const FNodeCostDefinition& Def)
{
    uint32 Hash = GetTypeHash(static_cast<uint8>(Def.CostSource));
    if (Def.CostSource == ENodeCostSource::ActorIntegerProperty)
    {
        Hash = HashCombine(Hash, GetTypeHash(Def.ActorResourcePropertyName));
    }
    else if (Def.CostSource == ENodeCostSource::GameplayAttributeValue)
    {
        Hash = HashCombine(Hash, GetTypeHash(Def.ResourceGameplayAttribute));
    }
    return Hash;
}

/**
 * @struct FNodeResourceCost
 * @brief Defines the amount of a resource required, potentially scaling with level.
 */
USTRUCT(BlueprintType)
struct FNodeResourceCost
{
    GENERATED_BODY()

    // The type of resource this cost uses.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
    FNodeCostDefinition CostDefinition;

    // A curve to define how the cost scales with the node's level.
    // X-axis: Node Level, Y-axis: Cost Amount.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
    TObjectPtr<UCurveFloat> CostCurve;

    // A flat amount of cost, used if CostCurve is not provided.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost", meta = (EditCondition = "CostCurve == nullptr"))
    int32 CostAmount = 1;

    /**
     * @brief Calculates the cost for a specific target level.
     * @param TargetLevel The level to calculate the cost for.
     * @param MaxLevel The maximum level of the node (for curve evaluation).
     * @return The calculated cost amount as an integer.
     */
    int32 GetCostForTargetLevel(int32 TargetLevel, int32 MaxLevel) const
    {
        if (CostCurve)
        {
            return FMath::RoundToInt(CostCurve->GetFloatValue(static_cast<float>(TargetLevel)));
        }
        return CostAmount;
    }
};

/**
 * @struct FResolvedNodeCost
 * @brief A simple struct to hold a calculated cost amount and its definition at runtime.
 */
USTRUCT(BlueprintType)
struct FResolvedNodeCost
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Resolved Cost")
    FNodeCostDefinition CostDefinition;

    UPROPERTY(BlueprintReadOnly, Category = "Resolved Cost")
    int32 ResolvedAmount = 0;

    FResolvedNodeCost() = default;
    FResolvedNodeCost(const FNodeCostDefinition& InDef, int32 InAmount)
        : CostDefinition(InDef), ResolvedAmount(InAmount) {}
};

#undef LOCTEXT_NAMESPACE
