// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Events/CrimsonSkillTree_NodeEvent_ApplyGameplayEffect.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimulationDataCollectorBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CRIMSONSKILLTREE_API USimulationDataCollectorBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Wrapper for AddData for a specific struct type
	UFUNCTION(BlueprintCallable, Category = "Simulation Data Collector", meta = (DisplayName = "Add Attribute Payload Data", ToolTip = "Adds an attribute payload to the collector."))
	static void AddAttributePayload(UPARAM(ref) FSimulationDataCollector& Collector, const FSkillTreeSimulatedAttributePayload& Payload);

	// Wrapper for GetAllDataOfType for a specific struct type
	// This returns an array of copies of the structs for Blueprint safety and ease of use.
	UFUNCTION(BlueprintCallable, Category = "Simulation Data Collector", meta = (DisplayName = "Get All Attribute Payloads", ToolTip = "Gets all attribute payloads from the collector."))
	static TArray<FSkillTreeSimulatedAttributePayload> GetAllAttributePayloads(const FSimulationDataCollector& Collector);

	// Wrapper for Clear
	UFUNCTION(BlueprintCallable, Category = "Simulation Data Collector", meta = (DisplayName = "Clear Simulation Data", ToolTip = "Clears all data from the collector."))
	static void ClearSimulationData(UPARAM(ref) FSimulationDataCollector& Collector);

	// Add more Add/Get wrappers here for other USTRUCT types you need to support in Blueprints
	// UFUNCTION(BlueprintCallable, Category = "Simulation Data Collector")
	// static void AddMyOtherStructData(UPARAM(ref) FSimulationDataCollector& Collector, const FMyOtherStruct& Payload);
	//
	// UFUNCTION(BlueprintCallable, Category = "Simulation Data Collector")
	// static TArray<FMyOtherStruct> GetAllMyOtherStructData(const FSimulationDataCollector& Collector);
};