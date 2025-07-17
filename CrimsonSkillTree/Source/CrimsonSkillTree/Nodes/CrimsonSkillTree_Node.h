// Copyright Crimson Sword Studio, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Cost/CrimsonSkillTree_NodeCost.h"
#include "CrimsonSkillTree_Node.generated.h"

struct FCrimsonSkillTree_SaveGameNodeState;
class UCrimsonSkillTree_ActivationConditionBase;
class UCrimsonSkillTreeManager;
class UCrimsonSkillTree_NodeEventBase;
class UCrimsonSkillTree_Edge;
class UCrimsonSkillTree;

/**
 * @enum ENodeState
 * @brief Defines the runtime state of a skill node.
 */
UENUM(BlueprintType)
enum class ENodeState : uint8
{
    UnSet         UMETA(DisplayName = "Unset"),      // The node has not been learned.
    Set           UMETA(DisplayName = "Set"),        // The node is learned and has available levels.
    Max           UMETA(DisplayName = "Max"),        // The node is learned and at its maximum level.
    Suppressed    UMETA(DisplayName = "Suppressed")  // The node is learned, but its prerequisites are not met, disabling its effects.
};

/**
 * @struct FCrimsonSkillTree_Node_UIData
 * @brief Structure holding UI-specific display data for a skill node.
 */
USTRUCT(BlueprintType)
struct FCrimsonSkillTree_Node_UIData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "UI Data") FText NodeName;
    UPROPERTY(BlueprintReadOnly, Category = "UI Data") FText NodeDescription;
    UPROPERTY(BlueprintReadOnly, Category = "UI Data") int32 NodeMaxLevel = 1;
    UPROPERTY(BlueprintReadOnly, Category = "UI Data") TObjectPtr<UTexture2D> NodeTexture = nullptr;
    UPROPERTY(BlueprintReadOnly, Category = "UI Data") ENodeState NodeState = ENodeState::UnSet;
};

/**
 * @struct FCrimsonSkillTree_NodeUIMessage
 * @brief Structure for carrying UI messages related to a skill node.
 */
USTRUCT(BlueprintType)
struct FCrimsonSkillTree_NodeUIMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "UIMessage") TObjectPtr<class UCrimsonSkillTree_Node> Node = nullptr;
    UPROPERTY(BlueprintReadOnly, Category = "UIMessage") FText Message;
    UPROPERTY(BlueprintReadOnly, Category = "UIMessage") float MessageLifetime = 1.5f;
};


/**
 * @class UCrimsonSkillTree_Node
 * @brief Represents a single, stateful node within a skill tree.
 */
UCLASS(Blueprintable)
class CRIMSONSKILLTREE_API UCrimsonSkillTree_Node : public UObject
{
    GENERATED_BODY()

public:
    UCrimsonSkillTree_Node();
    virtual ~UCrimsonSkillTree_Node();

    //~ Editor-Only Functions
#if WITH_EDITOR
    virtual bool IsNameEditable() const;
    virtual FLinearColor GetBackgroundColor() const;
    virtual FText GetNodeTitle() const;
    virtual void SetNodeTitle(const FText& NewTitle);
    virtual bool CanCreateConnection(UCrimsonSkillTree_Node* Other, FText& ErrorMessage);
    virtual bool CanCreateConnectionTo(UCrimsonSkillTree_Node* Other, int32 NumberOfChildrenNodes, FText& ErrorMessage);
    virtual bool CanCreateConnectionFrom(UCrimsonSkillTree_Node* Other, int32 NumberOfParentNodes, FText& ErrorMessage);
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
    FText GetContextMenuName() const { return ContextMenuName; }
    TSubclassOf<UCrimsonSkillTree> GetCompatibleSkillTreeClass() const { return CompatibleSkillTreeClass; }
#endif
    //~ End Editor-Only Functions

	/**
 	* @brief Gets the display name of the node. This is safe to call in both editor and runtime builds.
 	* @return The display name text.
 	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node")
	FText GetDisplayName() const;

    //~ Node Relationships & Structure
    UCrimsonSkillTree_Edge* GetEdge(UCrimsonSkillTree_Node* ChildNode);
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node") bool IsLeafNode() const;
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node") virtual bool IsRootNode() const;
    const TArray<UCrimsonSkillTree_Node*>& GetChildrenNodesArray() const { return ChildrenNodes; }
    const TArray<UCrimsonSkillTree_Node*>& GetParentNodesArray() const { return ParentNodes; }
    void SortChildrenByPredicate(TFunctionRef<bool(const UCrimsonSkillTree_Node& NodeA, const UCrimsonSkillTree_Node& NodeB)> Predicate) { ChildrenNodes.Sort(Predicate); }
    void SortParentByPredicate(TFunctionRef<bool(const UCrimsonSkillTree_Node& NodeA, const UCrimsonSkillTree_Node& NodeB)> Predicate) { ParentNodes.Sort(Predicate); }
    //~ End Node Relationships & Structure

    //~ Initialization & Context
    void SetSkillTreeManagerComponent(UCrimsonSkillTreeManager* InManagerComponent);
    UFUNCTION(BlueprintCallable, Category = "Node") UCrimsonSkillTree* GetSkillTree() const;
    UFUNCTION(BlueprintCallable, Category = "Node") void SetSkillTree(UCrimsonSkillTree* NewSkillTree);
    UFUNCTION(BlueprintCallable, Category = "Node") virtual void InitializeNodeState();
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node") UCrimsonSkillTreeManager* GetSkillTreeManagerComponent() const { return OwningManagerComponent; }
    //~ End Initialization & Context

    //~ State Checks (Can...)
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|State Checks") virtual bool CanActivate();
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|State Checks") virtual bool CanDeactivate();
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|State Checks") virtual bool CanIncrementLevel();
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|State Checks") virtual bool CanDecrementLevel();
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|State Checks") bool IsMaxLevel() const { return MaxLevel > 0 && CurrentLevel >= MaxLevel; }
    //~ End State Checks

    //~ State Changes (Do...)
    UFUNCTION(BlueprintCallable, Category = "Node|State Changes") virtual bool ActivateNode();
    UFUNCTION(BlueprintCallable, Category = "Node|State Changes") virtual bool DeactivateNode(bool bIsForceDeactivation = false);
    UFUNCTION(BlueprintCallable, Category = "Node|State Changes") virtual bool IncrementLevel();
    UFUNCTION(BlueprintCallable, Category = "Node|State Changes") virtual bool DecrementLevel();
    //~ End State Changes

    //~ Prerequisite & Condition Logic
    bool ArePrerequisitesMet() const;
    bool IsReachableFromRoot(const TSet<const UCrimsonSkillTree_Node*>& IgnoredNodes) const;
    bool ArePrerequisitesMetWithHypotheticalChange(AActor* OwnerContext, const TSet<const UCrimsonSkillTree_Node*>& HypotheticallyInactiveNodes, const TSet<const UCrimsonSkillTree_Node*>& HypotheticallyAlteredNodes = {}) const;
    UFUNCTION(BlueprintCallable, Category = "Node|State") bool UpdateNodeOverallState();
    //~ End Prerequisite & Condition Logic

    //~ UI & Display
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Node") FText GetDescription() const;
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node") virtual FCrimsonSkillTree_Node_UIData GetUIData() const { return NodeUIData; };
    UFUNCTION(BlueprintCallable, Category = "Node|Display", meta = (DisplayName = "Get Tooltip Descriptions")) void GetTooltipUIDescriptions(TArray<FText>& OutConditionDescriptions, TArray<FText>& OutEventDescriptions) const;
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|Cost") TArray<FText> GetFormattedNodeCosts() const;
    void BroadcastActivationFailureMessage(const FText& InMessage);
    UFUNCTION() virtual void InitNodeUIData();
    //~ End UI & Display

    //~ Save & Load
    void ApplyLoadedState(const FCrimsonSkillTree_SaveGameNodeState& SaveState);
    void SetNodeGUID(const FGuid& NewGUID) { NodeGuid = NewGUID; }
    FGuid GetNodeGUID() const { return NodeGuid; }
    void RestoreNodeToState(int32 TargetLevel, ENodeState TargetENodeState, bool bForceEventExecutionFromLevelZero = true);
    //~ End Save & Load

    //~ Cost Calculation
    TArray<FResolvedNodeCost> GetCostsForTargetLevel(int32 TargetLevel) const;
    TArray<FResolvedNodeCost> GetTotalCostsForAllActiveLevels() const;
    //~ End Cost Calculation

	//~ Graph Editor Data (Public for graph construction)
	void AddChildNode(UCrimsonSkillTree_Node* ChildNode) { if (ChildNode) ChildrenNodes.AddUnique(ChildNode); }
	void AddParentNode(UCrimsonSkillTree_Node* ParentNode) { if (ParentNode) ParentNodes.AddUnique(ParentNode); }
	void AddEdge(UCrimsonSkillTree_Node* ChildNode, UCrimsonSkillTree_Edge* Edge) { if (ChildNode && Edge) Edges.Add(ChildNode, Edge); }
	void ClearChildrenNodes() { ChildrenNodes.Empty(); }
	void ClearParentNodes() { ParentNodes.Empty(); }
	void ClearEdges() { Edges.Empty(); }
	//~ End Graph Editor Data

public:
	//~ Public Properties
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Node Identifier") FGuid NodeGuid;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Definition") FText ActualNodeTitle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Definition") FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Definition") FText NodeDescription;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Definition") FVector2D GamePosition;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Display") TObjectPtr<UTexture2D> NodeTexture;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Node|Config") FGameplayTag NodeTypeTag;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Node|Config", meta = (ClampMin = "1", UIMin = "1")) int32 MaxLevel;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node|Cost") TArray<FNodeResourceCost> NodeCosts;
    UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "Node|State") int32 CurrentLevel;
    UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "Node|State") ENodeState NodeState;
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Node|UI") TObjectPtr<class UCrimsonSkillTreeWidget_Node> RuntimeNodeWidget;
    UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Node|Events", meta = (DisplayName = "On Level Changed Events")) TArray<TObjectPtr<UCrimsonSkillTree_NodeEventBase>> OnLevelChangedEvents;
    UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Node|Conditions", meta = (DisplayName = "Activation Conditions")) TArray<TObjectPtr<UCrimsonSkillTree_ActivationConditionBase>> ActivationConditions;
	/** If true, this node is active by default and its costs should not be counted against the player's budget. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Node Configuration")
	bool bIsActiveByDefault = false;
	//~ End Public Properties

	//~ Delegates
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNodeStateChanged);
	UPROPERTY(BlueprintAssignable)
	FOnNodeStateChanged OnNodeStateChanged;
	//~ End Delegates
    
protected:
	//~ Protected Helper Functions
    void ExecuteLevelChangedEvents(int32 PreviousLevel);
    void HandleActiveMonitoringForConditions(bool bShouldMonitor);
    void OnIndividualActivationConditionChanged(bool bConditionIsNowMet);
    AActor* GetOwnerActorContext() const;
	//~ End Protected Helper Functions

protected:
	//~ Protected Properties
    UPROPERTY(Transient, BlueprintReadOnly, Category = "Node Internals")
    TObjectPtr<UCrimsonSkillTreeManager> OwningManagerComponent;
	//~ End Protected Properties

private:
	//~ Private Properties
    UPROPERTY() TObjectPtr<UCrimsonSkillTree> SkillTree;
    UPROPERTY() TArray<TObjectPtr<UCrimsonSkillTree_Node>> ParentNodes;
    UPROPERTY() TArray<TObjectPtr<UCrimsonSkillTree_Node>> ChildrenNodes;
    UPROPERTY() TMap<TObjectPtr<UCrimsonSkillTree_Node>, TObjectPtr<UCrimsonSkillTree_Edge>> Edges;
    UPROPERTY() FCrimsonSkillTree_Node_UIData NodeUIData;
	//~ End Private Properties

#if WITH_EDITORONLY_DATA
private:
	//~ Editor-Only Properties
    UPROPERTY() FText NodeTitle; // Used by graph editor
    UPROPERTY() TSubclassOf<UCrimsonSkillTree> CompatibleSkillTreeClass;
    UPROPERTY() FLinearColor BackgroundColor;
    UPROPERTY() FText ContextMenuName;
	//~ End Editor-Only Properties
#endif
};
