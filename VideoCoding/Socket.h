#pragma once

class Socket
{
public:
	static void connect();
  
	static void closeConnection();
	static void sendFrame(std::vector<char> message);
	static void sendMsg(std::vector<char>);
};

