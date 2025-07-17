// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrimsonSkillTree_Node.h" // Preserving original include

#include "CrimsonSkillTree_Edge.generated.h"

class UCrimsonSkillTree;
/**
 * */
UCLASS(Blueprintable)
class CRIMSONSKILLTREE_API UCrimsonSkillTree_Edge : public UObject
{
	GENERATED_BODY()

public:
	//~ Properties
	UPROPERTY()
	TObjectPtr<UCrimsonSkillTree> SkillTree;

	UPROPERTY(BlueprintReadOnly, Category = "Crimson|SkillTree|Edge")
	TObjectPtr<UCrimsonSkillTree_Node> StartNode;

	UPROPERTY(BlueprintReadOnly, Category = "Crimson|SkillTree|Edge")
	TObjectPtr<UCrimsonSkillTree_Node> EndNode;
	//~ End Properties

	//~ EditorOnly Properties
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode_Editor")
	bool bShouldDrawTitle = false;

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphNode_Editor")
	FText NodeTitle; // Preserved, even if unusual for an edge

	UPROPERTY(EditDefaultsOnly, Category = "GenericGraphEdge")
	FLinearColor EdgeColor = FLinearColor(0.6f, 0.0f, 0.0f, 0.3f);
#endif
	//~ End EditorOnly Properties

public:
	//~ Constructor & Destructor
	UCrimsonSkillTree_Edge();
	//~ UObject Interface
	virtual ~UCrimsonSkillTree_Edge();
	//~ End UObject Interface
	//~ End Constructor & Destructor

	//~ Public UFunctions
	UFUNCTION(BlueprintPure, Category = "Crimson|SkillTree")
	UCrimsonSkillTree* GetSkillTree();
	//~ End Public UFunctions

	//~ Editor Methods
#if WITH_EDITOR
	virtual FText GetNodeTitle() const { return NodeTitle; } // Preserved
	FLinearColor GetEdgeColor() const { return EdgeColor; }

	virtual void SetNodeTitle(const FText& NewTitle);
#endif
	//~ End Editor Methods
};