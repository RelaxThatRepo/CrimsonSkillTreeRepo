#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCrimsonSkillTree, All, All);

/**
 * @class FCrimsonSkillTreeModule
 * @brief The main module for the CrimsonSkillTree plugin.
 * @details This class handles the startup and shutdown logic for the module,
 * registering it with the engine.
 */
class FCrimsonSkillTreeModule : public IModuleInterface
{
public:

	// ~IModuleInterface
	/**
	 * @brief Called when the module is loaded into memory.
	 * @details This function is executed after the module is loaded, allowing for
	 * initialization of resources.
	 */
	virtual void StartupModule() override;

	/**
	 * @brief Called before the module is unloaded.
	 * @details This function is executed to clean up the module's resources before it is
	 * unloaded, especially important for dynamic reloading.
	 */
	virtual void ShutdownModule() override;
	// ~End IModuleInterface
};
