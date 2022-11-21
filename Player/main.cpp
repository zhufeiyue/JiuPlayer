#include <Common/Log.h>
#include <Common/Dic.h>

#ifdef DEBUG
int MyTestLog();
#endif

int main(int argc, char* argv[])
{
	InitLogger("abc");
	SetLogLevel(LogLevel::Info);

	LOG() << "123";
	LOG_WARNING() << "456";
	LOG_ERROR() << "ssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss";

	Dic dic;
	dic.insert("123", "10");
	dic.insert("456", 11);

	auto n = dic.get<std::string>("123");
	auto m = dic.get<int>("456");

	return 0;
}
