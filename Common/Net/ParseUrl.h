#pragma once
#include <string>

struct URL
{
	std::string schema; // http/https
	std::string host;
	std::string username;
	std::string password;
	int port = 0;
	std::string path;
	std::string query;
	std::string anchor;
};

int ParseUrl(const std::string&, URL&);