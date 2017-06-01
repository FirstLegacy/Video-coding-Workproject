#pragma once

#include <vector>

class Socket
{
public:
	static void connect();
	static void sendMessage(std::vector<char>);
};

