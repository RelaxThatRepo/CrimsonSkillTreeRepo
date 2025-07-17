// Copyright Crimson Sword Studio, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../Nodes/CrimsonSkillTree_Node.h" // Use relative path for better portability
#include "CrimsonSkillTree_SaveGame.generated.h"

/**
 * @struct FCrimsonSkillTree_SaveGameNodeState
 * @brief Holds the saved state of a single skill node.
 */
USTRUCT(BlueprintType)
struct FCrimsonSkillTree_SaveGameNodeState
{
	GENERATED_BODY()

	UPROPERTY() FGuid NodeGUID;
	UPROPERTY() int32 CurrentLevel = 0;
	UPROPERTY() ENodeState NodeState = ENodeState::UnSet;

	FCrimsonSkillTree_SaveGameNodeState() = default;

	// Constructor to easily create a save state from a node object.
	explicit FCrimsonSkillTree_SaveGameNodeState(const UCrimsonSkillTree_Node* Node)
	{
		if (Node)
		{
			NodeGUID = Node->NodeGuid;
			CurrentLevel = Node->CurrentLevel;
			NodeState = Node->NodeState;
		}
	}
};

/**
 * @struct FCrimsonSkillTree_SaveGameData
 * @brief Holds all saved node states for a single skill tree.
 */
USTRUCT(BlueprintType)
struct FCrimsonSkillTree_SaveGameData
{
	GENERATED_BODY()

	UPROPERTY() FGuid SkillTreeGUID;
	UPROPERTY() int32 SkillTreeVersion = 0;
	UPROPERTY() TArray<FCrimsonSkillTree_SaveGameNodeState> SavedNodeStates;
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
	UCrimsonSkillTree_SaveGame();
	
public:
	UPROPERTY() FString SaveSlotName;
	UPROPERTY() uint32 UserIndex = 0;
	UPROPERTY() TArray<FCrimsonSkillTree_SaveGameData> SkillTreesData;
};
