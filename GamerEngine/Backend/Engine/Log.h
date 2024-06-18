#pragma once


#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#ifdef _RELEASE
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum class LogLevel
{
	LOG_LEVEL_FATEL = 0,
	LOG_LEVEL_ERROR = 1,
	LOG_LEVEL_WARN = 2,
	LOG_LEVEL_INFO = 3,
	LOG_LEVEL_DEBUG = 4,
	LOG_LEVEL_TRACE = 5
} LogLevel;

void log_output(LogLevel aLevel, const char* aMessage, ...);

#define GE_FATAL(message, ...) log_output(LOG_LEVEL_FATEL, message ##__VAR_ARGS__)