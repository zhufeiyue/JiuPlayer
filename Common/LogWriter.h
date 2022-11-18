#pragma once

#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include "Log.h"

// unsafe
class LogWriter
{
public:
	virtual ~LogWriter() {}
	virtual int  Init(std::string);
	virtual void Do(std::string&, Logger::TimePoint&);

protected:
	std::ofstream m_fileLog;
};

struct WriteQueue
{
	int Write(std::string&, Logger::TimePoint&);
	int Flush();

	char              writeBuf[40960];
	Logger::TimePoint nearestTimepoint;
	std::deque<std::pair<std::string, Logger::TimePoint>> items;
};

class LogWriterAsync1 : public LogWriter
{
public:
	LogWriterAsync1();
	~LogWriterAsync1();

	int  Init(std::string) override;
	void Do(std::string&, Logger::TimePoint&) override;

private:
	std::mutex m_lock;
	WriteQueue m_queue[2];
	WriteQueue* pB = nullptr;
	WriteQueue* pF = nullptr;
};