#pragma once
#include <Common/Log.h>
#include <Common/EventLoop.h>
#include "ParseUrl.h"

class HttpClient : public std::enable_shared_from_this<HttpClient>
{
public:
	HttpClient();
	virtual ~HttpClient();

	HttpClient(const HttpClient&) = delete;
	HttpClient& operator=(const HttpClient&) = delete;
	HttpClient(HttpClient&&) = delete;
	HttpClient& operator=(HttpClient&&) = delete;

	int Request(std::string);

protected:
	virtual boost::asio::io_context& GetAsioContext();
	virtual void OnNetError(const system::error_code&);
	virtual void OnResolver(const system::error_code&, asio::ip::tcp::resolver::results_type);
	virtual void OnConnect(const system::error_code&);
	virtual void OnSendComplete(const system::error_code&, std::size_t count);
	virtual void OnRecvData(const system::error_code&, std::size_t count);

protected:
	virtual void SendRequestData();

protected:
	URL         m_url;
	std::string m_recvBuf;
	std::unique_ptr<asio::ip::tcp::resolver> m_pResolver;
	std::unique_ptr<asio::ip::tcp::socket>   m_pSocket;
};