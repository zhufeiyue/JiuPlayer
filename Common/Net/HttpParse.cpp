#include "HttpParse.h"

static int OnHeaderComplete(llhttp_t* t)
{
	t->data;
	return 0;
}

static int OnMessageComplete(llhttp_t* t)
{
	return 0;
}

int HttpParse::Init()
{
	llhttp_settings_init(&settings);

	settings.on_headers_complete = OnHeaderComplete;
	settings.on_message_complete = OnMessageComplete;

	return 0;
}

int HttpParse::Reset()
{
	return 0;
}

int HttpParse::Destroy()
{
	return 0;
}