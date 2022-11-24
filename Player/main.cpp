#include <Common/Log.h>
#include <Common/Dic.h>
#include <Common/EventLoop.h>
#include <Common/Net/HttpClient.h>

#ifdef DEBUG
int MyTestLog();
#endif

int main(int argc, char* argv[])
{
	InitLogger("player");
	SetLogLevel(LogLevel::Info);

	auto hc = std::make_shared<HttpClient>();
	hc->Request("http://500info.ueuo.com/");
	//hc->Request("https://cn.bing.com/search?q=boost+regex&qs=n&form=QBRE&sp=-1&pq=boost+regex&sc=10-11&sk=&cvid=B6B1C8CF896E43AFB94122631D1E02CD&ghsh=0&ghacc=0&ghpl=");

	EventLoop loop;
	loop.Queue().ScheduleTimer([&loop]() 
		{
			static int n = 0;
			LOG() << "sd " << n++;
			return CodeYes;
		}, 500, true);

	loop.Run();

	return 0;
}
