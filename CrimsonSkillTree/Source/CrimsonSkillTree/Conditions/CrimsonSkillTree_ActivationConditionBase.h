// Copyright Crimson Sword Studio, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CrimsonSkillTree_ActivationConditionBase.generated.h"

class UCrimsonSkillTree_Node;

/**
 * @struct FCrimsonSkillTree_NodeConditionFailureMessage
 * @brief A message payload for when a node's activation conditions fail,
 * typically used to trigger UI feedback on the nodes that are the source of the failure.
 */
USTRUCT(BlueprintType)
struct FCrimsonSkillTree_NodeConditionFailureMessage
{
	GENERATED_BODY()

	// A list of nodes that are the source of a condition failure.
	UPROPERTY(BlueprintReadWrite, Category = "Condition Failure")
	TArray<TObjectPtr<const UCrimsonSkillTree_Node>> FailedConditionNodes;
};

/**
 * @class UCrimsonSkillTree_ActivationConditionBase
 * @brief Abstract base class for all conditions that must be met for a skill node to be activated or remain active.
 * This class provides the core interface for checking conditions, simulating state changes, and monitoring game state.
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced, Abstract)
class CRIMSONSKILLTREE_API UCrimsonSkillTree_ActivationConditionBase : public UObject
{
	GENERATED_BODY()

public:
	UCrimsonSkillTree_ActivationConditionBase();

	//~ UObject Overrides
	virtual UWorld* GetWorld() const override;
	//~ End UObject Overrides

	//~ Public Interface
	/**
	 * @brief Called by the owning node when it becomes active, allowing this condition to start monitoring relevant game state.
	 * Derived classes MUST override this to bind to game events (e.g., attribute changes, tag changes) and call CheckAndBroadcastMetStateChange().
	 * @param InOwningNode The node that owns this condition instance.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Activation Condition")
	void BeginMonitoring(UCrimsonSkillTree_Node* InOwningNode);

	/**
	 * @brief Called by the owning node when it becomes inactive, allowing this condition to stop monitoring game state.
	 * Derived classes MUST override this to unbind from any game events they started listening to in BeginMonitoring.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Activation Condition")
	void EndMonitoring();
	
	/**
	 * @brief The primary function to check if this condition is currently met.
	 * @param OwningNode The node that this condition belongs to.
	 * @return True if the condition is met, false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Skill Activation Condition")
	bool IsConditionMet(const UCrimsonSkillTree_Node* OwningNode) const;

	/**
	 * @brief Simulates whether this condition would still be met if another node's benefits were altered.
	 * This is critical for predicting the outcome of unassigning a node without actually doing it.
	 * @param OwningConditionNode The node that this condition belongs to (e.g., Node B).
	 * @param OwnerActorContext The actor who owns the skill tree.
	 * @param NodeWhoseBenefitsAreAltered The node (e.g., Node A) whose benefits are being hypothetically changed.
	 * @param bBenefitsAreBeingLost True if simulating the loss of benefits (unassignment/level decrease).
	 * @return True if this condition would still be met under the hypothetical circumstances; false otherwise.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Activation Condition|Simulation")
	bool IsConditionStillMetIfBenefitsAltered(const UCrimsonSkillTree_Node* OwningConditionNode, AActor* OwnerActorContext, const UCrimsonSkillTree_Node* NodeWhoseBenefitsAreAltered, bool bBenefitsAreBeingLost) const;

	/**
	 * @brief Sets the cached owning node for this condition. Called by the node itself during initialization.
	 * @param InOwningNode The skill tree node that owns this condition.
	 */
	void SetCachedOwningNode(UCrimsonSkillTree_Node* InOwningNode);
	//~ End Public Interface

#if WITH_EDITOR
	/**
	 * @brief Provides a descriptive text for this condition for display in editor tooltips.
	 * @return A FText description of the condition's purpose.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Skill Activation Condition")
	FText GetEditorDescription() const;
#endif

	UFUNCTION(BlueprintNativeEvent, Category = "Skill Activation Condition")
	FText GetTooltipDescription() const;
	
	//~ Delegates
	/** Delegate broadcast when this specific condition's met status changes (e.g., from true to false). */
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnConditionMetStateChanged, bool /* bIsNowMet */);
	FOnConditionMetStateChanged OnConditionMetStateChanged;
	//~ End Delegates
	
protected:
	//~ Protected Properties
	// A weak pointer to the skill node that owns this condition instance. Set by BeginMonitoring.
	UPROPERTY(Transient)
	TWeakObjectPtr<UCrimsonSkillTree_Node> CachedOwningNode;

	// Stores the last known state of IsConditionMet to detect changes.
	bool bLastKnownMetState;
	//~ End Protected Properties

	//~ Protected Methods
	/**
	 * @brief Gets the owner actor associated with this condition via the cached owning node.
	 * @return The AActor that owns the skill tree system, or nullptr if not found.
	 */
	AActor* GetOwnerActor() const;
	
	/**
	 * @brief Checks the current met state and, if it has changed, broadcasts the OnConditionMetStateChanged delegate.
	 * This should be called by derived classes whenever a monitored game event occurs.
	 */
	void CheckAndBroadcastMetStateChange();
	//~ End Protected Methods

	//~ Native Implementations (for BlueprintNativeEvents)
	virtual void BeginMonitoring_Implementation(UCrimsonSkillTree_Node* InOwningNode);
	virtual void EndMonitoring_Implementation();
	virtual bool IsConditionMet_Implementation(const UCrimsonSkillTree_Node* OwningNode) const;
#if WITH_EDITOR
	virtual FText GetEditorDescription_Implementation() const;
#endif
	virtual bool IsConditionStillMetIfBenefitsAltered_Implementation(const UCrimsonSkillTree_Node* OwningConditionNode, AActor* OwnerActorContext, const UCrimsonSkillTree_Node* NodeWhoseBenefitsAreAltered, bool bBenefitsAreBeingLost) const;
	//~ End Native Implementations
};
