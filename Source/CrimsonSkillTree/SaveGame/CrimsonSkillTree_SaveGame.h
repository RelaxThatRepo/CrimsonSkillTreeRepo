#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../Nodes/CrimsonSkillTree_Node.h"
#include "CrimsonSkillTree_SaveGame.generated.h"

/**
 * @struct FCrimsonSkillTree_SaveGameNodeState
 * @brief Holds the saved state of a single skill node.
 */
USTRUCT(BlueprintType)
struct FCrimsonSkillTree_SaveGameNodeState
{
	GENERATED_BODY()

	FCrimsonSkillTree_SaveGameNodeState() = default;

	/**
	 * @brief Constructor to easily create a save state from a node object.
	 * @param Node The node to create the save state from.
	 */
	explicit FCrimsonSkillTree_SaveGameNodeState(const UCrimsonSkillTree_Node* Node)
	{
		if (Node)
		{
			NodeGUID = Node->NodeGuid;
			CurrentLevel = Node->CurrentLevel;
			NodeState = Node->NodeState;
		}
	}

	/** @brief The unique identifier of the node. */
	UPROPERTY()
	FGuid NodeGUID;

	/** @brief The level of the node when saved. */
	UPROPERTY()
	int32 CurrentLevel = 0;

	/** @brief The state of the node when saved. */
	UPROPERTY()
	ENodeState NodeState = ENodeState::UnSet;
};

/**
 * @struct FCrimsonSkillTree_SaveGameData
 * @brief Holds all saved node states for a single skill tree.
 */
USTRUCT(BlueprintType)
struct FCrimsonSkillTree_SaveGameData
{
	GENERATED_BODY()

	/** @brief The unique identifier of the skill tree. */
	UPROPERTY()
	FGuid SkillTreeGUID;

	/** @brief The version of the skill tree when saved. */
	UPROPERTY()
	int32 SkillTreeVersion = 0;

	/** @brief An array of all node states within this skill tree. */
	UPROPERTY()
	TArray<FCrimsonSkillTree_SaveGameNodeState> SavedNodeStates;
};

/**
 * @class UCrimsonSkillTree_SaveGame
 * @brief The SaveGame object that stores the state for all skill trees.
 */
UCLASS()
class CRIMSONSKILLTREE_API UCrimsonSkillTree_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/
	UCrimsonSkillTree_SaveGame();

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief The name of the save slot. */
	UPROPERTY()
	FString SaveSlotName;

	/** @brief The user index associated with this save. */
	UPROPERTY()
	uint32 UserIndex = 0;

	/** @brief An array containing the save data for each skill tree. */
	UPROPERTY()
	TArray<FCrimsonSkillTree_SaveGameData> SkillTreesData;
};
