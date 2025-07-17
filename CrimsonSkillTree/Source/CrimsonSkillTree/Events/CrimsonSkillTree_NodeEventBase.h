// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTree/SimulationDataCollector.h" // For FSimulationDataCollector
#include "UObject/Object.h"
#include "CrimsonSkillTree_NodeEventBase.generated.h"

class UCrimsonSkillTree_Node;
class AActor; // For the component owner
class UWorld; // For world context

/**
 * @class UCrimsonSkillTree_NodeEventBase
 * @brief Base class for events that are triggered by changes in a skill node's state (e.g., level up, level down, reset).
 * These events can implement various gameplay logic such as applying gameplay effects, granting abilities,
 * modifying attributes, or triggering other game systems.
 * This class is intended to be subclassed in C++ or Blueprint to create specific node events.
 * Instances are typically created as subobjects of UCrimsonSkillTree_Node.
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class CRIMSONSKILLTREE_API UCrimsonSkillTree_NodeEventBase : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * @brief Initializes the event with essential context from its owning node and the actor possessing the skill tree.
	 * This is typically called by the UCrimsonSkillTree_Node before this event is executed (e.g., before OnLevelUp/OnLevelDown).
	 * @param InOwnerActor The actor who owns the SkillTreeManagerComponent that manages the EmitterNode.
	 * @param InEmitterNode The skill node instance that is triggering this event.
	 */
	virtual void InitializeEventContext(AActor* InOwnerActor, UCrimsonSkillTree_Node* InEmitterNode);
	
	/**
	 * @brief Called when the EmitterNode's level increases or when it is initially activated.
	 * Subclasses should implement specific gameplay logic here.
	 * @param EmitterNode The skill node whose level changed.
	 * @param NewLevel The new current level of the EmitterNode.
	 * @param OldLevel The previous level of the EmitterNode (or 0 if newly activated).
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Node Event")
	void OnLevelUp(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel);

	/**
	 * @brief Called when the EmitterNode's level decreases or when it is deactivated.
	 * Subclasses should implement specific gameplay logic here, often reversing effects applied in OnLevelUp.
	 * @param EmitterNode The skill node whose level changed.
	 * @param NewLevel The new current level of the EmitterNode (or 0 if deactivated).
	 * @param OldLevel The previous level of the EmitterNode.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Node Event")
	void OnLevelDown(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel);

	/**
	 * @brief Called when the EmitterNode is forcefully reset to its unassigned/initial state (e.g., during a full skill tree respec).
	 * Subclasses should implement logic to completely reverse any persistent effects and clean up.
	 * @param EmitterNode The skill node that is being reset.
	 * @param PreviousLevel The previous level of the node before resetting.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Node Event")
	void OnNodeReset(UCrimsonSkillTree_Node* EmitterNode, int32 PreviousLevel);

	/**
	 * @brief Gets a descriptive text for this event, suitable for tooltips or UI displays.
	 * @param PotentialEmitterNode The node that would potentially emit this event. Can be used to provide context-specific descriptions.
	 * @return A FText description of the event's effect.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Skill Node Event|Display")
	FText GetTooltipDescription(const UCrimsonSkillTree_Node* PotentialEmitterNode) const;

	/**
 	 * @brief Allows this event to contribute to a simulation by populating a data collector with USTRUCTs
 	 * representing the simulated effects if this event were to be applied or reversed.
 	 * This is crucial for predicting the outcome of skill tree modifications without actually applying them.
 	 *
 	 * @param EventContextNode The node instance this event is associated with.
 	 * @param OriginalEffectiveLevel The level of the EventContextNode at which the event's effects were (or would be) active.
 	 * @param bSimulatingReversal True if simulating the removal/reversal of this event's effects (e.g., due to unassigning a node or leveling down).
 	 * False if simulating the application/gain of effects (e.g., assigning a node or leveling up).
 	 * @param OutDataCollector The collector to which this event should add its simulated data payloads (e.g., attribute changes, tag changes).
 	 * @note FSimulationDataCollector is a non-UObject class passed by reference for collecting various simulation data types.
 	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Node Event|Simulation")
	void PopulateSimulationData(
		const UCrimsonSkillTree_Node* EventContextNode,
		int32 OriginalEffectiveLevel,
		bool bSimulatingReversal,
		FSimulationDataCollector& OutDataCollector) const;

	/**
	 * @brief Overrides GetWorld() from UObject to provide a valid world context, typically derived from the OwnerActor.
	 * @return The UWorld this event object is associated with, or nullptr if unavailable.
	 */
	virtual UWorld* GetWorld() const override;

public:
	/** @brief A description of the event's effect, primarily for display in editor tooltips or UI. Can be set directly in the editor.
 	* If this has a description, it will take priority over the description gotten from GetTooltipDescription
 	* Leave empty to use the data from GetTooltipDescription.
 	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Description", meta=(MultiLine=true)) // Made BlueprintReadOnly
	FText EditorProvidedDescription;

protected:

	/** @brief Weak pointer to the actor that owns the SkillTreeManagerComponent, serving as the context for this event. Set via InitializeEventContext. */
	UPROPERTY(BlueprintReadOnly, Category = "Skill Node Event Context", Transient)
	TWeakObjectPtr<AActor> OwnerActor;

	/** @brief Weak pointer to the skill node that emitted (or would emit) this event. Set via InitializeEventContext. */
	UPROPERTY(BlueprintReadOnly, Category = "Skill Node Event Context", Transient)
	TWeakObjectPtr<UCrimsonSkillTree_Node> EmitterSkillNode;


public:
	//~ Native Implementations (for BlueprintNativeEvents) - These must be public for the generated thunks
	/** @cond DOXYGEN_IGNORE */ // Hide these from Doxygen as they are implementation details of BlueprintNativeEvent
	virtual void OnLevelUp_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel);
	virtual void OnLevelDown_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 NewLevel, int32 OldLevel);
	virtual void OnNodeReset_Implementation(UCrimsonSkillTree_Node* EmitterNode, int32 PreviousLevel);
	virtual FText GetTooltipDescription_Implementation(const UCrimsonSkillTree_Node* PotentialEmitterNode) const;
	virtual void PopulateSimulationData_Implementation(
		const UCrimsonSkillTree_Node* EventContextNode,
		int32 OriginalEffectiveLevel,
		bool bSimulatingReversal,
		FSimulationDataCollector& OutDataCollector) const;
	/** @endcond */
};