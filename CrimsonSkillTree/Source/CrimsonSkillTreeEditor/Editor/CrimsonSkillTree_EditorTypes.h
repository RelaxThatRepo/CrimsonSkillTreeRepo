#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "HAL/Platform.h"
#include "Math/NumericLimits.h"
#include "Math/Vector2D.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"
#include "Types/SlateVector2.h"

#include "CrimsonSkillTree_EditorTypes.generated.h"

class FName;

UCLASS()
class UCrimsonSkillTree_EditorTypes : public UObject
{
	GENERATED_UCLASS_BODY()
	
	static const FName PinCategory_MultipleNodes;
	static const FName PinCategory_SingleComposite;
	static const FName PinCategory_SingleTask;
	static const FName PinCategory_SingleNode;
};