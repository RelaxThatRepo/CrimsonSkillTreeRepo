// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CrimsonSkillTreeWidget_VisualNode.generated.h"


class USizeBox;
class UCrimsonSkillTree_VisualNode;
class UTextBlock;
class UBorder;

/**
 * @class UCrimsonSkillTreeWidget_VisualNode
 * @brief The base class for any widget that visually represents a UCrimsonSkillTree_VisualNode in the game UI.
 * This widget is designed to be placed on a UCanvasPanel.
 */
UCLASS(Abstract)
class CRIMSONSKILLTREE_API UCrimsonSkillTreeWidget_VisualNode : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Initializes the widget with data from the skill tree's visual node.
	 * The parent skill tree UI is responsible for calling this after creating the widget.
	 * @param InNodeData The data object from the skill tree asset.
	 */
	UFUNCTION(BlueprintCallable, Category = "Skill Tree Visual Node")
	virtual void InitializeFromNodeData(const UCrimsonSkillTree_VisualNode* InNodeData);

protected:
	/**
	 * @brief A reference to the source data from the skill tree asset.
	 * Can be used for further data binding in Blueprint.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill Tree Visual Node")
	TObjectPtr<const UCrimsonSkillTree_VisualNode> VisualNodeData;

	/**
 	* @brief The size bo. Designers should create a SizeBox in UMG and name it "SizeBox".
 	*/
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
	/**
	 * @brief The title text block. Designers should create a TextBlock in UMG and name it "TitleText".
	 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;

	/**
	 * @brief The main background element. Designers should create a Border in UMG and name it "BackgroundBorder".
	 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBorder> BackgroundBorder;
};
