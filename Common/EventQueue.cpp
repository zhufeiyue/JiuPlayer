#include "EventQueue.h"
#include "Log.h"

static void handleTimer(const boost::system::error_code& err,
	boost::asio::steady_timer* pTimer,
	Fn&& fn,
	int mill,
	bool repeat)
{
	int isContinue = fn();
	if (repeat && isContinue == CodeYes)
	{
		pTimer->expires_at(pTimer->expiry() + boost::asio::chrono::milliseconds(mill));
		pTimer->async_wait(
			[temp = std::move(fn), mill, repeat, pTimer](const boost::system::error_code& err) mutable
		{
			handleTimer(err, pTimer, std::forward<Fn>(temp), mill, repeat);
		});
	}
	else
	{
		delete pTimer;
	}
}

int EventQueue::ScheduleTimer(Fn&& fn, int mill, bool repeat)
{
	auto pTimer = new boost::asio::steady_timer(m_context, 
		boost::asio::chrono::milliseconds(mill));

	if (pTimer)
	{
		pTimer->async_wait([f = std::move(fn), mill, repeat, pTimer](const boost::system::error_code& err) mutable
		{
			handleTimer(err, pTimer, std::forward<Fn>(f), mill, repeat);
		});
	}

	return 0;
}

int EventQueue::PushEvent(Fn&& fn)
{
	if (fn)
	{
		boost::asio::post(m_context, std::forward<Fn>(fn));
	}
	return 0;
}

int EventQueue::PopEvent()
{
	boost::system::error_code err;

	auto n = m_context.poll(err);
	if (err)
	{
		LOG() << __FUNCTION__ << " " << err.message();
		return -1;
	}

	return n;
}