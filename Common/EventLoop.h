#pragma once
#include "EventQueue.h"

class EventQueue;
class EventLoop
{
public:
	EventLoop();
	~EventLoop();

	int Run();
	int Exit();
	bool IsRunning() const {
		return m_bRunning;
	}
	EventQueue& Queue() {
		return m_eventQueue;
	}

protected:
	EventQueue m_eventQueue;
	bool       m_bRunning = false;
};