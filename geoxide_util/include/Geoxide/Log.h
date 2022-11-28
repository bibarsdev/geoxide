
#ifndef __GX_LOG_H__
#define __GX_LOG_H__

namespace Geoxide {

class Log
{
public:
	static void Init(const std::string& name);

	static void Info(const std::string& msg);
	static void Trace(const std::string& msg);
	static void Warning(const std::string& msg);
	static void Error(const std::string& msg);
	static void Fatal(const std::string& msg);
};

}

#endif // !__GX_LOGGING_H__
