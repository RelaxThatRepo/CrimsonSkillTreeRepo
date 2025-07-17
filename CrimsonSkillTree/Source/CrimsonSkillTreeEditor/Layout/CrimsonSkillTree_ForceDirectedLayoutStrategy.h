// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTree_AutoLayoutStrategy.h"
#include "CrimsonSkillTree_ForceDirectedLayoutStrategy.generated.h"
/**
 * 
 */
UCLASS()
class CRIMSONSKILLTREEEDITOR_API UCrimsonSkillTree_ForceDirectedLayoutStrategy: public UCrimsonSkillTree_AutoLayoutStrategy
{
	GENERATED_BODY()
public:
	UCrimsonSkillTree_ForceDirectedLayoutStrategy();
	virtual ~UCrimsonSkillTree_ForceDirectedLayoutStrategy();

	virtual void Layout(UEdGraph* EdGraph) override;

protected:
	virtual FBox2D LayoutOneTree(UCrimsonSkillTree_Node* RootNode, const FBox2D& PreTreeBound);

protected:
	bool bRandomInit;
	float InitTemperature;
	float CoolDownRate;
};
