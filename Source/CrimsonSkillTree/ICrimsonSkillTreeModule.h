#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(CrimsonSkillTree, Log, All);

/**
 * @class IGenericGraphRuntime
 * @brief The public interface for the CrimsonSkillTreeModule.
 * @details Provides a singleton-like accessor for the module.
 */
class IGenericGraphRuntime : public IModuleInterface
{
public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/

	/**
	 * @brief Singleton-like access to this module's interface. This is just for convenience.
	 * @details Beware of calling this during the shutdown phase, as the module might have been unloaded already.
	 * @return Returns the singleton instance, loading the module on demand if needed.
	 */
	static IGenericGraphRuntime& Get()
	{
		return FModuleManager::LoadModuleChecked<IGenericGraphRuntime>("CrimsonSkillTreeModule");
	}

	/**
	 * @brief Checks to see if this module is loaded and ready.
	 * @details It is only valid to call Get() if IsAvailable() returns true.
	 * @return True if the module is loaded and ready to use.
	 */
	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("CrimsonSkillTreeModule");
	}
};
