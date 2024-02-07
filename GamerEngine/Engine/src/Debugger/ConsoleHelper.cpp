#include <GraphicsEngine.pch.h>
#include <Debugger/ConsoleHelper.h>
#include <string>




void ConsoleHelper::Init()
{
	if(!myHasInited)
	{
		myEnumNames.insert({ LogType::COUNT,		"ERROR" });
		myEnumNames.insert({ LogType::Error,		"ERROR" });
		myEnumNames.insert({ LogType::Warning,	"WARNING" });
		myEnumNames.insert({ LogType::Info,		"INFO" });
		myEnumNames.insert({ LogType::Success,	"SUCCESS" });
		myEnumNames.insert({ LogType::Failed,		"FAILED" });

		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);

		myHasInited = true;
	}
}

void ConsoleHelper::Startup()
{

}

void ConsoleHelper::Log(LogType aLogType, const std::string& aLogMessage)
{
	Init();

	int color;

	std::string logMessage = aLogMessage;

	switch(aLogType)
	{
	case LogType::Error:
		color = 4;
		break;
	case LogType::Warning:
		color = 6;
		break;
	case LogType::Info:
		color = 10;
		break;
	case LogType::Success:
		color = 10;
		break;
	case LogType::Failed:
		color = 4;
		break;
	case LogType::COUNT:
		color = 4;
		logMessage = "DON'T USE THE COUNT ENUM FOR LOG TYPES!";
		break;
	default:
		color = 15;
	}

	auto consoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleColor, color);
	std::cout << "[" << myEnumNames[aLogType] << "]" << " : ";

	SetConsoleTextAttribute(consoleColor, 15);
	std::cout << logMessage << "\n";
}

void ConsoleHelper::LogError(const std::string& aLogMessage)
{
	int color = 4;
	auto consoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consoleColor, color);
	std::cout << "[" << myEnumNames[LogType::Error] << "]" << " : ";

	SetConsoleTextAttribute(consoleColor, 15);
	std::cout << aLogMessage << "\n";
}
