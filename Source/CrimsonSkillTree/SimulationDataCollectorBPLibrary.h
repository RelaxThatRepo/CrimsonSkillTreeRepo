#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Events/CrimsonSkillTree_NodeEvent_ApplyGameplayEffect.h"
#include "SimulationDataCollectorBPLibrary.generated.h"

struct FSimulationDataCollector;

/**
 * @class USimulationDataCollectorBPLibrary
 * @brief Provides Blueprint-accessible wrappers for the FSimulationDataCollector struct.
 */
UCLASS()
class CRIMSONSKILLTREE_API USimulationDataCollectorBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	/**
	 * @brief Adds an attribute payload to the collector.
	 * @param Collector The data collector to modify.
	 * @param Payload The attribute payload data to add.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Simulation Data Collector", meta = (DisplayName = "Add Attribute Payload Data"))
	static void AddAttributePayload(UPARAM(ref) FSimulationDataCollector& Collector, const FSkillTreeSimulatedAttributePayload& Payload);

	/**
	 * @brief Gets all attribute payloads from the collector.
	 * @details This returns an array of copies of the structs for Blueprint safety and ease of use.
	 * @param Collector The data collector to query.
	 * @return An array of all attribute payloads found in the collector.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Simulation Data Collector", meta = (DisplayName = "Get All Attribute Payloads"))
	static TArray<FSkillTreeSimulatedAttributePayload> GetAllAttributePayloads(const FSimulationDataCollector& Collector);

	/**
	 * @brief Clears all data from the collector.
	 * @param Collector The data collector to clear.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree|Simulation Data Collector", meta = (DisplayName = "Clear Simulation Data"))
	static void ClearSimulationData(UPARAM(ref) FSimulationDataCollector& Collector);
};
