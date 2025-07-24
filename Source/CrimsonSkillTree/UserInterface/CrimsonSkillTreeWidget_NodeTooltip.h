#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrimsonSkillTreeWidget_NodeTooltip.generated.h"

class UCrimsonSkillTree_Node;

/**
 * @class UCrimsonSkillTreeWidget_NodeTooltip
 * @brief A dedicated widget for displaying detailed information about a skill node when hovered.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class CRIMSONSKILLTREE_API UCrimsonSkillTreeWidget_NodeTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/
	/**
	 * @brief Sets the skill node data for the tooltip to display.
	 * @details This is a BlueprintImplementableEvent, allowing all UI population logic to be handled in Blueprint graphs.
	 * @param InNodeData The node data to be displayed.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Skill Tree Tooltip")
	void SetSkillNodeData(UCrimsonSkillTree_Node* InNodeData);
	virtual void SetSkillNodeData_Implementation(UCrimsonSkillTree_Node* InNodeData);

protected:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief The skill node this tooltip is currently representing. */
	UPROPERTY(BlueprintReadOnly, Category = "Skill Tree Tooltip", meta = (ExposeOnSpawn = "true"))
	TObjectPtr<UCrimsonSkillTree_Node> NodeData;
};
