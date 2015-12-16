#pragma once
#include "define.h"
#include <string>
#include <array>
#include "boost/asio.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;
using namespace boost::asio;
using namespace boost::posix_time;

class Connection
	:public std::enable_shared_from_this<Connection>
{
public:
	Connection(const Connection&) = delete;
	Connection& operator=(const Connection&) = delete;

	explicit Connection(ip::tcp::socket theSocket);

	~Connection();

	void Start(void);
	void Stop(void);

private:
	void DoRead(void);
	void DoWrite(void);

private:
	ip::tcp::socket _sock;
	std::array<char, 8192> _buffer;
};

typedef std::shared_ptr<Connection> ConnectionPtr;