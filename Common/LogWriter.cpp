#include "LogWriter.h"

#ifdef _MSC_VER
#include <Windows.h>
#endif

int LogWriter::Init(std::string strFilePath)
{
	if (strFilePath.empty())
	{
		return CodeNo;
	}
	if (m_fileLog.is_open())
	{
		return CodeNo;
	}

	time_t now;
	time(&now);
	struct tm tmnow;
#ifdef _MSC_VER
	localtime_s(&tmnow, &now);
#else
	localtime_r(&now, &tmnow);
#endif

	std::stringstream ss;
	ss << strFilePath << '_'
		<< std::setw(2) << std::setfill('0') << (tmnow.tm_mon + 1) << '_'
		<< std::setw(2) << std::setfill('0') << tmnow.tm_mday
		<< ".log";

	m_fileLog.open(ss.str(), std::ofstream::out | std::ofstream::app);
	if (!m_fileLog.is_open())
	{
		return CodeNo;
	}

	std::clog.rdbuf(m_fileLog.rdbuf());

	return CodeYes;
}

void LogWriter::Do(std::string& strLog, Logger::TimePoint&)
{
	std::clog << strLog;

#ifdef _MSC_VER
	OutputDebugStringA(strLog.c_str());
#endif
}


int WriteQueue::Write(std::string& strLog, Logger::TimePoint& tp)
{
	// °´timepoint ½µÐò
	auto iter = items.begin();
	for (; iter != items.end(); ++iter)
	{
		if (tp >= iter->second)
			break;
	}

	items.insert(iter, std::make_pair(std::move(strLog), tp));

	return 0;
}

int WriteQueue::Flush()
{
	size_t count = 0;

	for (auto iter = items.rbegin(); iter != items.rend(); ++iter)
	{

#ifdef _MSC_VER
		OutputDebugStringA(iter->first.c_str());
#endif

		if (count + iter->first.length() >= sizeof(writeBuf))
		{
			writeBuf[count] = '\0';
			std::clog << writeBuf;
			count = 0;
		}

		memcpy(writeBuf + count, iter->first.c_str(), iter->first.length());
		count += iter->first.length();
	}

	if (count != 0)
	{
		writeBuf[count] = '\0';
		std::clog << writeBuf;
	}

	items.clear();

	return 0;
}


LogWriterAsync1::LogWriterAsync1()
{
}

LogWriterAsync1::~LogWriterAsync1()
{
	pB->Flush();
	pF->Flush();
}

int LogWriterAsync1::Init(std::string s)
{
	pB = &m_queue[0];
	pF = &m_queue[1];
	pF->nearestTimepoint = pB->nearestTimepoint = Logger::Clock::now();

	return LogWriter::Init(s);
}

void LogWriterAsync1::Do(std::string& strLog, Logger::TimePoint& tp)
{
	std::lock_guard<std::mutex> guard(m_lock);

	if (tp <= pB->nearestTimepoint)
	{
		pB->Write(strLog, tp);
	}
	else
	{
		pF->Write(strLog, tp);
		if (tp > pF->nearestTimepoint)
		{
			pF->nearestTimepoint = tp;
			if (pF->nearestTimepoint - pB->nearestTimepoint > std::chrono::milliseconds(1000))
			{
				pB->Flush();
				
				std::swap(pF, pB);
			}
		}
	}
}