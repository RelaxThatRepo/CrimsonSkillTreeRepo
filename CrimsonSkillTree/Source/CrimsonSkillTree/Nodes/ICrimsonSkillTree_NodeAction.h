// Copyright Crimson Sword Studio, 2024. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICrimsonSkillTree_NodeAction.generated.h"

class UCrimsonSkillTree_Node;

UINTERFACE(MinimalAPI, Blueprintable)
class UCrimsonSkillTree_NodeAction : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class ICrimsonSkillTree_NodeAction
 * @brief An interface for defining actions that can be performed on a skill tree node during traversal.
 */
class CRIMSONSKILLTREE_API ICrimsonSkillTree_NodeAction
{
	GENERATED_BODY()

public:
	/**
	 * @brief Executes an action on the given skill tree node.
	 * @param Node The node to perform the action on.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill Tree|Node Action")
	void ExecuteAction(UCrimsonSkillTree_Node* Node);
};