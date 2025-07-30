
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICrimsonSkillTreeInterface.generated.h"

class UCrimsonSkillTreeManager;

UINTERFACE(MinimalAPI, Blueprintable)
class UCrimsonSkillTreeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class ICrimsonSkillTreeInterface
 * @brief Interface for any actor that can own and manage a Crimson Skill Tree.
 */
class CRIMSONSKILLTREE_API ICrimsonSkillTreeInterface
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Retrieves the Actor that is the logical owner of the skill tree's effects (typically the Pawn or Character).
	 * @return A pointer to the owning Actor.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Crimson Skill Tree")
	AActor* GetSkillTreeOwner() const;

	/**
 	* @brief Retrieves the Actor that is the logical owner of the skill tree's effects (typically the Pawn or Character).
 	* @return A pointer to the owning Actor.
 	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Crimson Skill Tree")
	ACharacter* GetSkillTreeOwnerCharacter() const;
	
	/**
	 * @brief Retrieves the Skill Tree Manager component from this object.
	 * @return A pointer to the UCrimsonSkillTreeManager component.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Crimson Skill Tree")
	UCrimsonSkillTreeManager* GetSkillTreeManagerComponent() const;
	
};