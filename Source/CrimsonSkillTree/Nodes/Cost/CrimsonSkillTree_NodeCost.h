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
 * @details This is a hashable struct, allowing it to be used as a key in TMaps.
 */
USTRUCT(BlueprintType)
struct FNodeCostDefinition
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	/**
	 * @brief Equality operator for TMap support.
	 * @param Other The other definition to compare against.
	 * @return True if the definitions are for the same resource.
	 */
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

	/**
	 * @brief Helper function to get a clean resource name, with fallbacks.
	 * @return The user-facing name of the resource.
	 */
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

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief The source from which the resource is derived (e.g., an actor property or a gameplay attribute). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Definition")
	ENodeCostSource CostSource = ENodeCostSource::NoCost;

	/** @brief User-facing name for this resource (e.g., "Skill Points"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Definition")
	FText ResourceUserFacingName;

	/** @brief The name of an integer property on the owner actor to use as the resource pool. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Definition", meta = (EditCondition = "CostSource == ENodeCostSource::ActorIntegerProperty"))
	FName ActorResourcePropertyName;

	/** @brief The gameplay attribute to use as the resource pool. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Definition", meta = (EditCondition = "CostSource == ENodeCostSource::GameplayAttributeValue"))
	FGameplayAttribute ResourceGameplayAttribute;
};

/**
 * @brief Hash function for FNodeCostDefinition to allow its use as a key in TMaps.
 * @param Def The FNodeCostDefinition to hash.
 * @return The calculated hash value.
 */
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

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/
	/**
	 * @brief Calculates the cost for a specific target level.
	 * @param TargetLevel The level to calculate the cost for.
	 * @param MaxLevel The maximum level of the node (for curve evaluation).
	 * @return The calculated cost amount as an integer.
	 */
	int32 GetCostForTargetLevel(int32 TargetLevel, int32 MaxLevel) const
	{
		float FoundValue = 0.f;
		if (CostCurveHandle.Eval(static_cast<float>(TargetLevel), &FoundValue, FString("GetCostForTargetLevel")))
		{
			return FMath::RoundToInt(FoundValue);
		}
		return CostAmount;
	}


public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief The type of resource this cost uses. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
	FNodeCostDefinition CostDefinition;

	/** @brief Should this cost be determined by a curve instead of a flat value? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
	bool bUseCurveCost = false;

	/** @brief Handle to a curve in a Curve Table. This is only used if bUseCurveCost is true. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost", meta = (EditCondition = "bUseCurveCost"))
	FCurveTableRowHandle CostCurveHandle;

	/** @brief A flat amount of cost. This is only used if bUseCurveCost is false. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost", meta = (EditCondition = "!bUseCurveCost"))
	int32 CostAmount = 1;
};

/**
 * @struct FResolvedNodeCost
 * @brief A simple struct to hold a calculated cost amount and its definition at runtime.
 */
USTRUCT(BlueprintType)
struct FResolvedNodeCost
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/
	FResolvedNodeCost() = default;
	FResolvedNodeCost(const FNodeCostDefinition& InDef, int32 InAmount)
		: CostDefinition(InDef), ResolvedAmount(InAmount) {}

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief The definition of the resource cost. */
	UPROPERTY(BlueprintReadOnly, Category = "Resolved Cost")
	FNodeCostDefinition CostDefinition;

	/** @brief The calculated amount of the cost. */
	UPROPERTY(BlueprintReadOnly, Category = "Resolved Cost")
	int32 ResolvedAmount = 0;
};

#undef LOCTEXT_NAMESPACE
