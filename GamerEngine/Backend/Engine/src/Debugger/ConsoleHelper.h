#pragma once
#include <unordered_map>


enum class LogType
{
	Error,
	Warning,
	Info,
	Success,
	Failed,
	COUNT
};

class ConsoleHelper
{
public:


	static void Init();
	static void Startup();


	static void Log(LogType aLogType, const char* format, ...);
	static void LogError(const std::string& aLogMessage);
	static void Clear();

	static std::vector<std::string>& GetLogs();

private:


	
	inline static bool myHasInited = false;
	inline static std::vector<std::string> myLogs;
	inline static std::unordered_map<LogType, std::string> myEnumNames;

};