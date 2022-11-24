#include "ParseUrl.h"
#include <algorithm>
#include <boost/regex.hpp>

int ParseUrl(const std::string& strURL, URL& url)
{
	//	scheme = $2
	//	authority = $4
	//	path = $5
	//	query = $7
	//	fragment = $9

	// https://stackoverflow.com/questions/5620235/cpp-regular-expression-to-validate-url

	boost::regex r(
		R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)", 
		boost::regex_constants::extended);
	boost::smatch result;

	if (!boost::regex_match(strURL, result, r))
	{
		return -1;
	}

	url = URL();
	url.schema = result[2];
	if (url.schema.empty())
	{
		return -1;
	}
	std::transform(url.schema.begin(), url.schema.end(), url.schema.begin(), tolower);

	url.host = result[4];
	if (url.host.empty())
	{
		return -1;
	}
	else
	{
		auto pos = url.host.find(':');
		if (pos != std::string::npos)
		{
			url.port = atoi(url.host.c_str() + pos + 1);
		}
		else
		{
			url.port = 0;
		}
	}

	url.path = result[5];
	if (url.path.empty())
	{
		url.path = "/";
	}
	else
	{
		url.query = result[7];
		url.anchor = result[9];
	}

	return 0;
}