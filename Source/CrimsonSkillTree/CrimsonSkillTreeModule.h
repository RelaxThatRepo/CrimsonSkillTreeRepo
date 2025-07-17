// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

/** @brief Log category for the CrimsonSkillTree module. */
DECLARE_LOG_CATEGORY_EXTERN(LogCrimsonSkillTree, All, All);

/**
 * @class FCrimsonSkillTreeModule
 * @brief The main module class for the CrimsonSkillTree plugin.
 * This class handles the startup and shutdown logic for the module.
 */
class FCrimsonSkillTreeModule : public IModuleInterface
{
public:

	// ~ IModuleInterface
	/**
	 * @brief Called when the module is starting up.
	 * This is where you can initialize any resources your module needs.
	 */
	virtual void StartupModule() override;

	/**
	 * @brief Called when the module is shutting down.
	 * This is where you should clean up any resources your module has allocated.
	 * This function may be called during shutdown to clean up your module. For modules that support dynamic reloading,
	 * we call this function before unloading the module.
	 */
	virtual void ShutdownModule() override;
	// ~End IModuleInterface
};