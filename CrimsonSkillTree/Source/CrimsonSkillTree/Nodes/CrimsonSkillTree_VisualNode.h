// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CrimsonSkillTree_VisualNode.generated.h"

class UCommonBorderStyle;
class UCommonTextStyle;
class UCrimsonSkillTreeWidget_VisualNode;
/**
 * 
 */
UCLASS(Blueprintable)
class CRIMSONSKILLTREE_API UCrimsonSkillTree_VisualNode : public UObject
{
	GENERATED_BODY()
	
public:
	/** @brief The display title for this visual node. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Node")
	FText Title;

	/** @brief The position of the top-left corner of this node in the graph. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Node")
	FVector2D Position;

	/** @brief The size (width and height) of this node. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Node")
	FVector2D Size;

	/** The widget class to spawn for this visual node in the game UI. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Node")
	TSubclassOf<UCrimsonSkillTreeWidget_VisualNode> WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Node|Overrides")
	TSubclassOf<UCommonTextStyle> TextStyle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual Node|Overrides")
	TSubclassOf<UCommonBorderStyle> BorderStyle;
};
