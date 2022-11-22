#include "EventLoop.h"
#include "EventQueue.h"
#include "Log.h"

EventLoop::EventLoop()
{
}

EventLoop::~EventLoop()
{
}

int EventLoop::Run()
{
	LOG() << "event loop start";
	if (m_bRunning)
	{
		return -1;
	}
	m_bRunning = true;

	int n = 0;
	while (IsRunning())
	{
		n = m_eventQueue.PopEvent();
		if (n == 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		else if (n < 0)
		{
			m_bRunning = false;
		}
	}

	LOG() << "event loop stop";

	return 0;
}

int EventLoop::Exit()
{
	m_bRunning = false;
	return 0;
}