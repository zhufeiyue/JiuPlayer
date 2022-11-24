#pragma once
#ifdef _MSC_VER
#include <sdkddkver.h>
#endif
#include <functional>
#include <boost/asio.hpp>

using namespace boost;
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

	asio::io_context& Context() {
		return m_context;
	}
	int ScheduleTimer(Fn&&, int mill, bool repeat);
	int PushEvent(Fn&&);
	int PopEvent();

private:
	asio::io_context m_context;
};