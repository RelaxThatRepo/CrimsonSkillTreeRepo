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
	UnSet UMETA(DisplayName = "Unset"),
	Set UMETA(DisplayName = "Set"),
	Max UMETA(DisplayName = "Max"),
	Suppressed UMETA(DisplayName = "Suppressed")
};

/**
 * @struct FCrimsonSkillTree_Node_UIData
 * @brief Structure holding UI-specific display data for a skill node.
 */
USTRUCT(BlueprintType)
struct FCrimsonSkillTree_Node_UIData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "UI Data")
	FText NodeName;

	UPROPERTY(BlueprintReadOnly, Category = "UI Data")
	FText NodeDescription;

	UPROPERTY(BlueprintReadOnly, Category = "UI Data")
	int32 NodeMaxLevel = 1;

	UPROPERTY(BlueprintReadOnly, Category = "UI Data")
	TObjectPtr<UTexture2D> NodeTexture = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "UI Data")
	ENodeState NodeState = ENodeState::UnSet;
};

/**
 * @struct FCrimsonSkillTree_NodeUIMessage
 * @brief Structure for carrying UI messages related to a skill node.
 */
USTRUCT(BlueprintType)
struct FCrimsonSkillTree_NodeUIMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "UIMessage")
	TObjectPtr<class UCrimsonSkillTree_Node> Node = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "UIMessage")
	FText Message;

	UPROPERTY(BlueprintReadOnly, Category = "UIMessage")
	float MessageLifetime = 1.5f;
};


/**
 * @class UCrimsonSkillTree_Node
 * @brief Represents a single, stateful node within a skill tree.
 */
UCLASS(BlueprintType, EditInlineNew)
class CRIMSONSKILLTREE_API UCrimsonSkillTree_Node : public UObject
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	// ~Construction
	// =============================================================================================================
	UCrimsonSkillTree_Node();
	virtual ~UCrimsonSkillTree_Node();

	// ~UObject Overrides
	// =============================================================================================================
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }

	// ~Editor-Only Functions
	// =============================================================================================================
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

	// ~Core Accessors
	// =============================================================================================================
	/**
	 * @brief Gets the display name of the node.
	 * @return The display name text.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node")
	FText GetDisplayName() const;

	/**
	 * @brief Gets the unique identifier for this node.
	 * @return The node's GUID.
	 */
	FGuid GetNodeGUID() const { return NodeGuid; }

	// ~Node Relationships & Structure
	// =============================================================================================================
	/**
	 * @brief Gets the edge connecting this node to a specific child.
	 * @param ChildNode The child node.
	 * @return The edge object, or nullptr if no direct edge exists.
	 */
	UCrimsonSkillTree_Edge* GetEdge(UCrimsonSkillTree_Node* ChildNode);

	/**
	 * @brief Checks if this node is a leaf node (has no children).
	 * @return True if this is a leaf node.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|Structure")
	bool IsLeafNode() const;

	/**
	 * @brief Checks if this node is the root of the skill tree.
	 * @return True if this is the root node.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|Structure")
	virtual bool IsRootNode() const;

	/**
	 * @brief Gets the array of child nodes.
	 * @return A const reference to the children nodes array.
	 */
	const TArray<UCrimsonSkillTree_Node*>& GetChildrenNodesArray() const { return ChildrenNodes; }

	/**
	 * @brief Gets the array of parent nodes.
	 * @return A const reference to the parent nodes array.
	 */
	const TArray<UCrimsonSkillTree_Node*>& GetParentNodesArray() const { return ParentNodes; }

	/**
	 * @brief Sorts the children array based on a predicate.
	 * @param Predicate The sorting predicate.
	 */
	void SortChildrenByPredicate(TFunctionRef<bool(const UCrimsonSkillTree_Node& NodeA, const UCrimsonSkillTree_Node& NodeB)> Predicate) { ChildrenNodes.Sort(Predicate); }

	/**
	 * @brief Sorts the parent array based on a predicate.
	 * @param Predicate The sorting predicate.
	 */
	void SortParentByPredicate(TFunctionRef<bool(const UCrimsonSkillTree_Node& NodeA, const UCrimsonSkillTree_Node& NodeB)> Predicate) { ParentNodes.Sort(Predicate); }

	// ~Initialization & Context
	// =============================================================================================================
	void SetSkillTreeManagerComponent(UCrimsonSkillTreeManager* InManagerComponent);
	UFUNCTION(BlueprintCallable, Category = "Node|Context")
	UCrimsonSkillTree* GetSkillTree() const;
	UFUNCTION(BlueprintCallable, Category = "Node|Context")
	void SetSkillTree(UCrimsonSkillTree* NewSkillTree);
	UFUNCTION(BlueprintCallable, Category = "Node|State")
	virtual void InitializeNodeState();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|Context")
	UCrimsonSkillTreeManager* GetSkillTreeManagerComponent() const { return OwningManagerComponent; }

	// ~State Checks (Can...)
	// =============================================================================================================
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|State Checks")
	virtual bool CanActivate();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|State Checks")
	virtual bool CanDeactivate();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|State Checks")
	virtual bool CanIncrementLevel();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|State Checks")
	virtual bool CanDecrementLevel();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|State Checks")
	bool IsMaxLevel() const { return MaxLevel > 0 && CurrentLevel >= MaxLevel; }

	// ~State Changes (Do...)
	// =============================================================================================================
	UFUNCTION(BlueprintCallable, Category = "Node|State Changes")
	virtual bool ActivateNode();
	UFUNCTION(BlueprintCallable, Category = "Node|State Changes")
	virtual bool DeactivateNode(bool bIsForceDeactivation = false);
	UFUNCTION(BlueprintCallable, Category = "Node|State Changes")
	virtual void ShutdownNode();
	UFUNCTION(BlueprintCallable, Category = "Node|State Changes")
	virtual bool IncrementLevel();
	UFUNCTION(BlueprintCallable, Category = "Node|State Changes")
	virtual bool DecrementLevel();

	// ~Prerequisite & Condition Logic
	// =============================================================================================================
	bool ArePrerequisitesMet() const;
	bool IsReachableFromRoot(const TSet<const UCrimsonSkillTree_Node*>& IgnoredNodes) const;
	bool ArePrerequisitesMetWithHypotheticalChange(AActor* OwnerContext, const TSet<const UCrimsonSkillTree_Node*>& HypotheticallyInactiveNodes, const TSet<const UCrimsonSkillTree_Node*>& HypotheticallyAlteredNodes = {}) const;
	UFUNCTION(BlueprintCallable, Category = "Node|State")
	bool UpdateNodeOverallState();

	// ~UI & Display
	// =============================================================================================================
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Node|Display")
	FText GetDescription() const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|Display")
	virtual FCrimsonSkillTree_Node_UIData GetUIData() const { return NodeUIData; };
	UFUNCTION(BlueprintCallable, Category = "Node|Display", meta = (DisplayName = "Get Tooltip Descriptions"))
	void GetTooltipUIDescriptions(TArray<FText>& OutConditionDescriptions, TArray<FText>& OutEventDescriptions) const;
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Node|Cost")
	TArray<FText> GetFormattedNodeCosts() const;
	void BroadcastActivationFailureMessage(const FText& InMessage);
	UFUNCTION()
	virtual void InitNodeUIData();

	// ~Save & Load
	// =============================================================================================================
	void ApplyLoadedState(const FCrimsonSkillTree_SaveGameNodeState& SaveState);
	void SetNodeGUID(const FGuid& NewGUID) { NodeGuid = NewGUID; }
	void RestoreNodeToState(int32 TargetLevel, ENodeState TargetENodeState, bool bForceEventExecutionFromLevelZero = true);

	// ~Cost Calculation
	// =============================================================================================================
	TArray<FResolvedNodeCost> GetCostsForTargetLevel(int32 TargetLevel) const;
	TArray<FResolvedNodeCost> GetTotalCostsForAllActiveLevels() const;

	// ~Graph Editor Data
	// =============================================================================================================
	void AddChildNode(UCrimsonSkillTree_Node* ChildNode);
	void AddParentNode(UCrimsonSkillTree_Node* ParentNode);
	void AddEdge(UCrimsonSkillTree_Node* ChildNode, UCrimsonSkillTree_Edge* Edge) { if (ChildNode && Edge) Edges.Add(ChildNode, Edge); }
	void ClearChildrenNodes() { ChildrenNodes.Empty(); }
	void ClearParentNodes() { ParentNodes.Empty(); }
	void ClearEdges() { Edges.Empty(); }

	// ~Replication
	// =============================================================================================================
	UFUNCTION()
	void OnRep_NodeStateChanged();

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/

	// ~Delegates
	// =============================================================================================================
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNodeStateChanged);
	UPROPERTY(BlueprintAssignable)
	FOnNodeStateChanged OnNodeStateChanged;

	// ~Definition Properties
	// =============================================================================================================
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Node Identifier")
	FGuid NodeGuid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Definition")
	FText ActualNodeTitle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Node|Definition")
	FText DisplayName;
	UPROPERTY()
	FVector2D GamePosition;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node|Display")
	TObjectPtr<UTexture2D> NodeTexture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Node|Config", meta = (DisplayName = "Node Widget Type Tag"))
	FGameplayTag NodeTypeTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Node|Config", meta = (ClampMin = "1", UIMin = "1"))
	int32 MaxLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Node|Config")
	bool bIsActiveByDefault = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Node|Cost")
	TArray<FNodeResourceCost> NodeCosts;
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Node|Events", meta = (DisplayName = "On Level Changed Events"))
	TArray<TObjectPtr<UCrimsonSkillTree_NodeEventBase>> OnLevelChangedEvents;
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Node|Conditions", meta = (DisplayName = "Activation Conditions"))
	TArray<TObjectPtr<UCrimsonSkillTree_ActivationConditionBase>> ActivationConditions;

	// ~Runtime State
	// =============================================================================================================
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "Node|State")
	int32 CurrentLevel;
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = "Node|State")
	ENodeState NodeState;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Node|UI")
	TObjectPtr<class UCrimsonSkillTreeWidget_Node> RuntimeNodeWidget;

protected:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/
	void ExecuteLevelChangedEvents(int32 PreviousLevel);
	void HandleActiveMonitoringForConditions(bool bShouldMonitor);
	void OnIndividualActivationConditionChanged(bool bConditionIsNowMet);
	AActor* GetOwnerActorContext() const;

protected:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Node Internals")
	TObjectPtr<UCrimsonSkillTreeManager> OwningManagerComponent;

private:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	UPROPERTY()
	TObjectPtr<UCrimsonSkillTree> SkillTree;
	UPROPERTY()
	TArray<TObjectPtr<UCrimsonSkillTree_Node>> ParentNodes;
	UPROPERTY()
	TArray<TObjectPtr<UCrimsonSkillTree_Node>> ChildrenNodes;
	UPROPERTY()
	TMap<TObjectPtr<UCrimsonSkillTree_Node>, TObjectPtr<UCrimsonSkillTree_Edge>> Edges;
	UPROPERTY()
	FCrimsonSkillTree_Node_UIData NodeUIData;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	FText NodeTitle;
	UPROPERTY()
	TSubclassOf<UCrimsonSkillTree> CompatibleSkillTreeClass;
	UPROPERTY()
	FLinearColor BackgroundColor;
	UPROPERTY()
	FText ContextMenuName;
#endif
};
