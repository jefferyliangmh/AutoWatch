#include "stdafx.h"
#include "Connection.h"


Connection::Connection(ip::tcp::socket theSocket)
	:_sock(std::move(theSocket))
{
}


Connection::~Connection()
{
}

void Connection::Start(void)
{
	DoRead();
}

void Connection::Stop(void)
{
	_sock.close();
}

void Connection::DoRead(void)
{
	auto self(shared_from_this());
	_sock.async_read_some(buffer(_buffer),
		[this, self](boost::system::error_code ec, std::size_t transferredBytes)
	{
		if (!ec)
		{

		}
		else if (ec != error::operation_aborted)
		{

		}
	});
}

void Connection::DoWrite(void)
{
}
