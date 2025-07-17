// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "CrimsonSkillTree_GraphNode_Visual.generated.h"

class UCrimsonSkillTree_VisualNode;

/**
 * @class UCrimsonSkillTree_GraphNode_Visual
 * @brief Editor graph node for a visual-only element. It has no pins and cannot be connected.
 * It holds a reference to a runtime UCrimsonSkillTree_VisualNode.
 */
UCLASS()
class CRIMSONSKILLTREEEDITOR_API UCrimsonSkillTree_GraphNode_Visual : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UCrimsonSkillTree_GraphNode_Visual();

	/** @brief Pointer to the runtime visual node data object. */
	UPROPERTY(VisibleAnywhere, Instanced, Category = "Visual Node")
	TObjectPtr<UCrimsonSkillTree_VisualNode> VisualNode;

	//~ UEdGraphNode interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PrepareForCopying() override;
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual bool CanDuplicateNode() const override { return true; }
	//~ End UEdGraphNode interface

	/** Sets the size of the underlying visual node */
	void SetSize(const FVector2D& NewSize);
};