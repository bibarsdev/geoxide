
#include "Geoxide/Log.h"

namespace Geoxide {

	HANDLE gStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	void Log::trace(const std::string& msg)
	{
		// Blue
		SetConsoleTextAttribute(gStdHandle, 0x03);
		log("Trace", msg);
	}

	void Log::info(const std::string& msg)
	{
		// Light Gray
		SetConsoleTextAttribute(gStdHandle, 0x07);
		log("Info", msg);
	}

	void Log::warning(const std::string& msg)
	{
		// Yellow
		SetConsoleTextAttribute(gStdHandle, 0x0E);
		log("Warning", msg);
	}

	void Log::error(const std::string& msg)
	{
		// light red
		SetConsoleTextAttribute(gStdHandle, 0x0C);
		log("Error", msg);
	}

	void Log::fatal(const std::string& msg)
	{
		// red
		SetConsoleTextAttribute(gStdHandle, 0x04);
		log("Fatal", msg);

		// Light Gray
		SetConsoleTextAttribute(gStdHandle, 0x07);

		std::cout << "Press any key to exit...";
		std::cin.get();

		exit(1);
	}

}
