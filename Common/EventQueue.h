#pragma once
#ifdef _MSC_VER
#include <sdkddkver.h>
#endif
#include <functional>
#include <boost/asio.hpp>

typedef std::function<int()> Fn;

class EventQueue
{
public:
	EventQueue() = default;
	~EventQueue() = default;
	EventQueue(const EventQueue&) = delete;
	EventQueue& operator=(const EventQueue&) = delete;
	EventQueue(EventQueue&&) = delete;
	EventQueue& operator=(EventQueue&&) = delete;

	int ScheduleTimer(Fn&&, int mill, bool repeat);
	int PushEvent(Fn&&);
	int PopEvent();

private:
	boost::asio::io_context m_context;
};