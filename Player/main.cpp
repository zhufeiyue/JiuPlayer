#include <Common/Log.h>

int main(int argc, char* argv[])
{
	InitLogger("abc");
	SetLogLevel(LogLevel::Info);

	LOG() << "123";
	LOG_WARNING() << "456";
	LOG_ERROR() << "ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss";


	return 0;
}