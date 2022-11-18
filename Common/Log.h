#pragma once
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

enum class LogLevel
{
	Info = 1,
	Warning = 10,
	Error = 20
};

enum ResultCode
{
	CodeInvalidParam = -2,
	CodeNo = -1,
	CodeYes = 0,
	CodeOK = CodeYes,
	CodeAgain = 1,
	CodeRejection,
	CodeEnd
};

int InitLogger(const char*);
int SetLogLevel(LogLevel);
int GetLogLevel(LogLevel&);

struct LogStream
{
	template<typename T>
	LogStream& operator<< (T t)
	{
		if (bLog)
		{
			ss << t;
		}
		return *this;
	}

	bool              bLog = true;
	std::stringstream ss;
};

class Logger
{
public:
	typedef std::chrono::system_clock Clock;
	typedef Clock::time_point TimePoint;

public:
	Logger(int level);
	~Logger();
	LogStream& Log();

private:
	LogStream stream;
	TimePoint timepoint;
};

#if defined LOG || defined LOG_WARNING || defined LOG_ERROR || defined LOG_CUSTOM
#error "redefine log macro"
#endif

#define LOG     Logger((int)LogLevel::Info).Log
#define LOG_WARNING   Logger((int)LogLevel::Warning).Log
#define LOG_ERROR     Logger((int)LogLevel::Error).Log
#define LOG_CUSTOM(x) Logger(x).Log