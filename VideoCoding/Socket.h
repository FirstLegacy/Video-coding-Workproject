#pragma once

#include <vector>

class Socket
{
public:
	static void connect();
  
	static void closeConnection();
	static void SendFrame(std::vector<char> message);
	static void send(std::vector<char>);
};

