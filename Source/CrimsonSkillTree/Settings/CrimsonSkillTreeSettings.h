#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CrimsonSkillTreeSettings.generated.h"

/**
 * @enum ECSTLogType
 * @brief Defines the verbosity level for the plugin's logging output.
 */
UENUM()
enum class ECSTLogType : uint8
{
	All,
	Log,
	Info,
	Verbose,
	Warning,
	Error,
	None,
};

/**
 * @class UCrimsonSkillTreeSettings
 * @brief Defines the runtime settings for the Crimson Skill Tree plugin, configurable in Project Settings.
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Crimson Skill Tree"))
class CRIMSONSKILLTREE_API UCrimsonSkillTreeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/
	UCrimsonSkillTreeSettings();

public:
	/****************************************************************************************************************
	* Properties                                                           *
	****************************************************************************************************************/
	/** @brief If true, enables all logging from the CSTLog helper class across the plugin. */
	UPROPERTY(Config, EditAnywhere, Category = "Logging", meta = (DisplayName = "Enable Plugin Logging"))
	bool bEnableGlobalLogging;

	/** @brief The specific type of log messages to display when logging is enabled. */
	UPROPERTY(Config, EditAnywhere, Category = "Logging", meta = (DisplayName = "Log Type to Display", EditCondition = "bEnableGlobalLogging"))
	ECSTLogType LogType;
};
