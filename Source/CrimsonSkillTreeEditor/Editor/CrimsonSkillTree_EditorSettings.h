// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CrimsonSkillTree_EditorSettings.generated.h"

class UCrimsonSkillTree_AutoLayoutStrategy;
/**
 * 
 */
UCLASS()
class CRIMSONSKILLTREEEDITOR_API UCrimsonSkillTree_EditorSettings  : public UObject
{
	GENERATED_BODY()

public:
	UCrimsonSkillTree_EditorSettings();
	virtual ~UCrimsonSkillTree_EditorSettings();

	UPROPERTY(EditDefaultsOnly, Category = "AutoArrange")
	float OptimalDistance;

	UPROPERTY(EditDefaultsOnly, AdvancedDisplay, Category = "AutoArrange")
	TSubclassOf<UCrimsonSkillTree_AutoLayoutStrategy> AutoLayoutStrategy;

	UPROPERTY()
	int32 MaxIteration;

	UPROPERTY()
	bool bFirstPassOnly;

	UPROPERTY()
	bool bRandomInit;

	UPROPERTY()
	float InitTemperature;

	UPROPERTY()
	float CoolDownRate;
	
};
