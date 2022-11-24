#pragma once
#include "llhttp/llhttp.h"

struct HttpParse
{
	llhttp_t          parser;
	llhttp_settings_t settings;

	virtual int Init();
	virtual int Reset();
	virtual int Destroy();
};