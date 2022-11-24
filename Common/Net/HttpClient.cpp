#include "HttpClient.h"
#include <boost/stacktrace.hpp>

struct HttpHelper
{
	std::thread thread;
	EventLoop eventLoop;

	HttpHelper()
	{
		thread = std::thread([this] 
			{
				eventLoop.Run();
			});
	}

	~HttpHelper()
	{
		eventLoop.Exit();

		if (thread.joinable())
		{
			thread.join();
		}
	}
};

boost::asio::io_context& HttpClient::GetAsioContext()
{
	static std::unique_ptr< HttpHelper> httpHelper(new HttpHelper());
	return httpHelper->eventLoop.Queue().Context();
}

HttpClient::HttpClient()
{

}

HttpClient::~HttpClient()
{
};

int HttpClient::Request(std::string strURL)
{
	if (0 != ParseUrl(strURL, m_url))
	{
		LOG() << "parse url failed:" << strURL;
		return CodeNo;
	}

	m_pResolver.reset(new asio::ip::tcp::resolver(GetAsioContext()));
	if (!m_pResolver)
	{
		return CodeNo;
	}

	m_pResolver->async_resolve(
		m_url.host,
		m_url.port != 0 ? std::to_string(m_url.port) : m_url.schema,
		[pThis = shared_from_this()](const system::error_code& err, asio::ip::tcp::resolver::results_type result) {
			pThis->OnResolver(err, result);
		});

	return CodeOK;
}

void HttpClient::OnNetError(const system::error_code& err)
{
	LOG() << err.message();
}

void HttpClient::OnResolver(const system::error_code& err, asio::ip::tcp::resolver::results_type result)
{
	if (err)
	{
		OnNetError(err);
		return;
	}

	if (m_url.schema == "http")
	{
		m_pSocket.reset(new asio::ip::tcp::socket(GetAsioContext()));
		m_pSocket->async_connect(*result, [pThis=shared_from_this()](const system::error_code& err) {
			pThis->OnConnect(err);
		});
	}
}

void HttpClient::OnConnect(const system::error_code& err)
{
	if (err)
	{
		OnNetError(err);
		return;
	}

	SendRequestData();
}

void HttpClient::OnSendComplete(const system::error_code& err, std::size_t count)
{
	if (err)
	{
		OnNetError(err);
		return;
	}

	m_recvBuf.clear();

	if (m_pSocket)
	{
		asio::async_read(*m_pSocket, asio::dynamic_buffer(m_recvBuf),
			[pThis = shared_from_this()](const system::error_code& err, std::size_t n)
		{
			pThis->OnRecvData(err, n);
		});
	}
}

void HttpClient::OnRecvData(const system::error_code& err, std::size_t count)
{
	if (err)
	{
		OnNetError(err);
		return;
	}
}

void HttpClient::SendRequestData()
{
	std::stringstream ss;
	std::string strRequestURI;

	strRequestURI = m_url.path;
	if (!m_url.query.empty())
	{
		strRequestURI += "?" + m_url.query;
	}

	ss << "GET" << ' ' << strRequestURI << ' ' << "HTTP/1.1" << "\r\n";
	ss << "Host: " << m_url.host << "\r\n";
	ss << "Connection: keep-alive" << "\r\n";
	ss << "User-Agent: Mozilla / 5.0 (Windows NT 10.0; Win64; x64; rv:97.0) Gecko / 20100101 Firefox / 97.0" << "\r\n";
	ss << "Accept: text/plain, */*; q=0.01" << "\r\n";
	ss << "Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2" << "\r\n";
	ss << "\r\n";

	auto pRequestData = std::make_shared<std::string>(ss.str());
	auto requestBuf = asio::buffer(pRequestData->data(), pRequestData->length());
	auto requestLenAtLeast = asio::transfer_at_least(pRequestData->length());

	auto requestCb = [pRequestData, pThis = shared_from_this()](const system::error_code& err, std::size_t n)
	{
		pThis->OnSendComplete(err, n);
	};

	if (m_pSocket)
	{
		asio::async_write(*m_pSocket, requestBuf, requestLenAtLeast, std::move(requestCb));
	}
}