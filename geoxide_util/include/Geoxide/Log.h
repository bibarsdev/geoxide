
#ifndef __GX_LOG_H__
#define __GX_LOG_H__

namespace Geoxide {

class Log
{
public:
	Log(const std::string& name) : mName(name) {}

	void info(const std::string& msg);
	void trace(const std::string& msg);
	void warning(const std::string& msg);
	void error(const std::string& msg);
	void fatal(const std::string& msg);

private:
	void log(const std::string& type, const std::string& msg);

private:
	std::string mName;
};

}

#define GX_DECLARE_LOG(name) inline static Log sLog = Log(name)

#endif // !__GX_LOGGING_H__
