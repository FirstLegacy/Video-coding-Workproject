#pragma once
class Socket
{
public:
	static void connect();
	static void sendMessage(std::vector<uint8_t>);
};

