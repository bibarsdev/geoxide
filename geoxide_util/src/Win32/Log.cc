
#include "Geoxide/Log.h"

#include <Psapi.h>

namespace Geoxide {

	HANDLE gStdHandle = 0;
	std::string gName = "Geoxide";

	void LogMessage(const std::string& type, const std::string& msg, WORD color)
	{
		auto t = std::time(nullptr);
		auto tm = std::localtime(&t);

		auto timestamp = std::put_time(tm, "[%H:%M:%S]");

		std::cout << timestamp << " [" << gName << "] [";

		SetConsoleTextAttribute(gStdHandle, color);
		std::cout << type;
		SetConsoleTextAttribute(gStdHandle, 0x07);

		std::cout << "] " << msg << '\n';
	}

	void Log::Init(const std::string& name)
	{
		gName = name;
		gStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	void Log::Trace(const std::string& msg)
	{
		// Blue
		LogMessage("Trace", msg, 0x03);
	}

	void Log::Info(const std::string& msg)
	{
		// Green
		LogMessage("Info", msg, 0x0A);
	}

	void Log::Warning(const std::string& msg)
	{
		// Yellow
		LogMessage("Warning", msg, 0x0E);
	}

	void Log::Error(const std::string& msg)
	{
		// Light red
		LogMessage("Error", msg, 0x0C);
	}

	void Log::Fatal(const std::string& msg)
	{
		// Red
		LogMessage("Fatal", msg, 0x04);

		std::cout << "Press any key to exit...";
		std::cin.get();

		exit(1);
	}


}
