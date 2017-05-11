#pragma once

#include<vector>

class Socket
{
public:
	static void connect();
	static void closeConnection();
	static void send(std::vector<char>);
};

