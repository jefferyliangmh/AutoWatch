#pragma once
#include "Connection.h"
#include <set>
class ConnectionManager
{
public:
	ConnectionManager(const ConnectionManager&) = delete;
	ConnectionManager& operator=(const ConnectionManager&) = delete;

	ConnectionManager();

private:
	set<ConnectionPtr> _connections;
};

