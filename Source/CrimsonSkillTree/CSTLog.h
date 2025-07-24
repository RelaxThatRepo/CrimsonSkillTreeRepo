#pragma once

#include "CoreMinimal.h"
#include "Settings/CrimsonSkillTreeSettings.h"

/**
 * @class CSTLog
 * @brief Helper class to check logging settings from the project configuration.
 * @details The actual logging is performed by the macros defined in this file.
 */
class CRIMSONSKILLTREE_API CSTLog
{
public:
	/****************************************************************************************************************
	* Functions                                                            *
	****************************************************************************************************************/
	CSTLog() = delete;
	CSTLog(const CSTLog&) = delete;
	CSTLog& operator=(const CSTLog&) = delete;

	/**
	 * @brief Checks if global logging is enabled in the Project Settings.
	 * @return True if logging is enabled, false otherwise.
	 */
	static bool IsLoggingEnabled()
	{
#if WITH_EDITOR
		if (const UCrimsonSkillTreeSettings* Settings = GetDefault<UCrimsonSkillTreeSettings>())
		{
			return Settings->bEnableGlobalLogging;
		}
#endif
		return false;
	}

	/**
	 * @brief Gets the current log type filter from the Project Settings.
	 * @return The configured ECSTLogType.
	 */
	static ECSTLogType GetLogType()
	{
#if WITH_EDITOR
		if (const UCrimsonSkillTreeSettings* Settings = GetDefault<UCrimsonSkillTreeSettings>())
		{
			return Settings->LogType;
		}
#endif
		return ECSTLogType::None;
	}
};

// ~Logging Macros
// =============================================================================================================

#if !defined(CST_LOG)
#define CST_LOG(Format, ...) \
	do { \
		if (CSTLog::IsLoggingEnabled() && (CSTLog::GetLogType() == ECSTLogType::All || CSTLog::GetLogType() == ECSTLogType::Log)) \
		{ \
			UE_LOG(LogCrimsonSkillTree, Log, Format, ##__VA_ARGS__); \
		} \
	} while(0)
#endif

#if !defined(CST_INFO)
#define CST_INFO(Format, ...) \
	do { \
		if (CSTLog::IsLoggingEnabled() && (CSTLog::GetLogType() == ECSTLogType::All || CSTLog::GetLogType() == ECSTLogType::Info)) \
		{ \
			UE_LOG(LogCrimsonSkillTree, Log, Format, ##__VA_ARGS__); \
		} \
	} while(0)
#endif

#if !defined(CST_VERBOSE)
#define CST_VERBOSE(Format, ...) \
	do { \
		if (CSTLog::IsLoggingEnabled() && (CSTLog::GetLogType() == ECSTLogType::All || CSTLog::GetLogType() == ECSTLogType::Verbose)) \
		{ \
			UE_LOG(LogCrimsonSkillTree, Verbose, Format, ##__VA_ARGS__); \
		} \
	} while(0)
#endif

#if !defined(CST_WARN)
#define CST_WARN(Format, ...) \
	do { \
		if (CSTLog::IsLoggingEnabled() && (CSTLog::GetLogType() == ECSTLogType::All || CSTLog::GetLogType() == ECSTLogType::Warning)) \
		{ \
			UE_LOG(LogCrimsonSkillTree, Warning, Format, ##__VA_ARGS__); \
		} \
	} while(0)
#endif

#if !defined(CST_ERROR)
#define CST_ERROR(Format, ...) \
	do { \
		if (CSTLog::IsLoggingEnabled() && (CSTLog::GetLogType() == ECSTLogType::All || CSTLog::GetLogType() == ECSTLogType::Error)) \
		{ \
			UE_LOG(LogCrimsonSkillTree, Error, Format, ##__VA_ARGS__); \
		} \
	} while(0)
#endif

#if !defined(CST_SCREEN_MSG)
#define CST_SCREEN_MSG(Key, Time, Color, Format, ...) \
	do { \
		if (CSTLog::IsLoggingEnabled() && CSTLog::GetLogType() == ECSTLogType::All) \
		{ \
			if (GEngine) \
			{ \
				GEngine->AddOnScreenDebugMessage(Key, Time, Color, FString::Printf(TEXT(Format), ##__VA_ARGS__)); \
			} \
		} \
	} while(0)
#endif
