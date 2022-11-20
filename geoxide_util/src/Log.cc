
#include "Geoxide/Log.h"

namespace Geoxide {

	void Log::log(const std::string& type, const std::string& msg)
	{
		auto t = std::time(nullptr);
		auto tm = std::localtime(&t);

		auto timestamp = std::put_time(tm, "[%H:%M:%S]");

		std::cout << timestamp << " [" << mName << "] [" << type << "] " << msg << '\n';
	}


}
