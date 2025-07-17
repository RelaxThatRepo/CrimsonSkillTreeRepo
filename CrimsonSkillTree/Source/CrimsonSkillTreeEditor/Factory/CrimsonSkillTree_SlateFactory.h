// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CrimsonSkillTree_SlateFactory.generated.h"

/**
 * 
 */
UCLASS(Abstract, MinimalAPI)
class UCrimsonSkillTree_SlateNodeFactory : public UObject
{
	GENERATED_BODY()
public:
	// Implement this in derived classes to create the specific SGraphNode widget
	virtual TSharedPtr<SGraphNode> CreateWidget(UEdGraphNode* InNode) const
	{
		// Base implementation should not be called directly or return a default/null
		checkf(false, TEXT("CreateWidget must be implemented in derived class."));
		return nullptr;
	}
};


UCLASS(Abstract, MinimalAPI)
class UCrimsonSkillTree_SlateEdgeFactory : public UObject
{
	GENERATED_BODY()
public:
	// Implement this in derived classes to create the specific SGraphNode (SGraphEdge) widget
	virtual TSharedPtr<SGraphNode> CreateWidget(UEdGraphNode* InNode) const
	{
		checkf(false, TEXT("CreateWidget must be implemented in derived class."));
		return nullptr;
	}
};


UCLASS(MinimalAPI)
class UCrimsonSkillTree_SlateNodeFactory_Default : public UCrimsonSkillTree_SlateNodeFactory
{
	GENERATED_BODY()
public:
	virtual TSharedPtr<SGraphNode> CreateWidget(UEdGraphNode* InNode) const override;
};

UCLASS(MinimalAPI) // Not abstract
class UCrimsonSkillTree_SlateEdgeFactory_Default : public UCrimsonSkillTree_SlateEdgeFactory
{
	GENERATED_BODY()
public:
	virtual TSharedPtr<SGraphNode> CreateWidget(UEdGraphNode* InNode) const override;
};