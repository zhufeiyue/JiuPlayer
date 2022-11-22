#include "EventQueue.h"
#include "Log.h"

struct TimerHelper
{
	int  interval;
	bool repeat;
	Fn   fn;
	boost::asio::steady_timer timer;
	
	TimerHelper(boost::asio::io_context& context, int mill) :
		interval(mill),
		timer(context, boost::asio::chrono::milliseconds(mill))
	{
	}
};

static void handleTimer(std::shared_ptr<TimerHelper> p, const boost::system::error_code& err)
{
	if (err)
	{
		LOG() << err.message();
		return;
	}

	if (p->repeat && (p->fn)() == CodeYes)
	{
		p->timer.expires_at(p->timer.expiry() + std::chrono::milliseconds(p->interval));
		p->timer.async_wait([p](const boost::system::error_code& err) 
			{
				handleTimer(p, err);
			});
	}
}

int EventQueue::ScheduleTimer(Fn&& fn, int mill, bool repeat)
{
	auto p = std::make_shared<TimerHelper>(m_context, mill);
	if (!fn || !p)
	{
		LOG() << __FUNCTION__;
		return -1;
	}

	p->fn = std::move(fn);
	p->repeat = repeat;
	p->timer.async_wait([p](const boost::system::error_code& err) 
		{
			handleTimer(p, err);
		});

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

	return (int)n;
}