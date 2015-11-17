#pragma once
#include "Define.h"
#include <string>

enum SeverityLevel
{
	Notification = 0,
	Debug,
	Warning,
	Error,
	Critical
};

class LOGGER_API Logger
{
public:
	class LoggerImpl;

	static void Log(const std::string& message);
	static void Log(const std::string& message, const SeverityLevel& level);

private:
	Logger(void);
	~Logger(void);

	Logger(const Logger&);
	const Logger& operator=(const Logger&);
};