#include <Common/Log.h>
#include <Common/Dic.h>
#include <Common/EventLoop.h>

#ifdef DEBUG
int MyTestLog();
#endif

int main(int argc, char* argv[])
{
	InitLogger("player");
	SetLogLevel(LogLevel::Info);

	EventLoop loop;

	loop.Queue().ScheduleTimer([&loop]() 
		{
			static int n = 0;
			LOG() << "sd " << n++;
			if (n > 200)
			{
				loop.Exit();
			}

			return CodeYes;
		}, 40, true);

	loop.Run();

	return 0;
}
