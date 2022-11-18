#include "Log.h"
#include "LogWriter.h"

#ifdef _MSC_VER
#include <Windows.h>
#endif

static LogLevel                   gLogLevel = LogLevel::Info;
static std::unique_ptr<LogWriter> gLogWriter;

int InitLogger(const char* logFileNamePrefix)
{
	gLogWriter = std::make_unique<LogWriter>();
	gLogWriter = std::make_unique<LogWriterAsync1>();

	if (CodeYes == gLogWriter->Init(logFileNamePrefix))
	{
		return CodeYes;
	}
	else
	{
		gLogWriter.reset();
		return CodeNo;
	}
}

int SetLogLevel(LogLevel level)
{
	gLogLevel = level;
	return 0;
}

int GetLogLevel(LogLevel& level)
{
	level = gLogLevel;
	return 0;
}

static
std::string FormatLogTimePrefix(Logger::TimePoint& now)
{
	now.time_since_epoch().count();

	auto now_time = Logger::Clock::to_time_t(now);
	auto diff = now - Logger::Clock::from_time_t(now_time);
	auto mill = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();

	struct tm now_tm;
#ifdef _MSC_VER
	localtime_s(&now_tm, &now_time);
#else
	localtime_r(&now_t, &now_tm);
#endif

	char buf[32] = { 0 };
	snprintf(buf, sizeof(buf), "%02d-%02d %02d:%02d:%02d.%03d ",
		now_tm.tm_mon + 1,
		now_tm.tm_mday,
		now_tm.tm_hour,
		now_tm.tm_min,
		now_tm.tm_sec,
		(int)mill);
	
	return buf;
}

Logger::Logger(int level)
{
	stream.bLog = level >= static_cast<int>(gLogLevel);
}

Logger::~Logger()
{
	if (stream.bLog)
	{
		stream.ss << std::endl;

		auto strLog = stream.ss.str();
		if (strLog.length() > 2048)
		{
			return;
		}

		if (gLogWriter)
		{
			gLogWriter->Do(std::move(strLog), timepoint);
		}
	}
}

LogStream& Logger::Log()
{
	if (stream.bLog)
	{
		timepoint = Clock::now();
		stream << FormatLogTimePrefix(timepoint);
	}
	return stream;
}

#ifdef _DEBUG

#include <atomic>
#include <algorithm>
#include <thread>
#include <vector>
#include <deque>
#include <list>
int MyTestLog()
{
	static std::atomic<int> g = 0;

	const int kCount = 1;
	std::thread t[kCount];

	for (int i = 0; i < kCount; ++i)
	{
		t[i] = std::thread([i]() {
			for (int j = 0; j < 500; ++j)
			{
				LOG() << "thread " << i << " print " << ++g;
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		});
	}

	for (int i = 0; i < kCount; ++i)
	{
		if (t[i].joinable())
			t[i].join();
		LOG() << i;
	}

;	return CodeYes;
}

class A
{
public:
	int a = 10;
	std::string s = "s";

	A() = default;

	A(const A& r)
	{
		a = r.a;
		s = r.s;
		std::cout << "copy construct A" << std::endl;
	}

	A(A&& r)
	{
		a = r.a;
		s = std::move(r.s);
		std::cout << "move construct A" << std::endl;
	}

	A& operator= (const A& r)
	{
		if (this != &r)
		{
			a = r.a;
			s = r.s;
		}

		std::cout << "copy operator = A" << std::endl;
		return *this;
	}

	A& operator= (A&& r)
	{
		if (this != &r)
		{
			a = r.a;
			s = std::move(r.s);
		}

		std::cout << "move operator = A" << std::endl;

		return *this;
	}
};

int MyTestListSort()
{
	std::string s = "asc";

	std::pair<int, std::string> pa = std::make_pair(10, std::move(s));
	std::pair<int, A> pa1 = std::make_pair(20, A());

	std::deque<A> la;
	std::list<A> la1;

	for (int i = 0; i < 100; ++i)
	{
		A a;
		a.a = rand() % 20000;

		la.push_back(a);
		//la.push_back(std::move(a));
	}

	std::sort(la.begin(), la.end(), [](A& l, A& r) 
		{
			return l.a < r.a;
		});


	return 0;
}

#endif