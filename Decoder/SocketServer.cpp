#include "stdafx.h"
#include "SocketServer.h"
#include "DisplayBuffer.h"

#include <iostream>
#include <Ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib") // Winsock library.

#define PORT 22205
#define MAX_SIZE 1460

WSADATA wsaData;
SOCKET mySocket;
sockaddr_in myAddress, otherAddress;

// void init(int port)
void SocketServer::listen()
{
	std::cout << "Socket initializing . . . ";

	// Create socket
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		WSACleanup();
		throw std::runtime_error("Failed to startup WSA.");
	}

	mySocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (mySocket == INVALID_SOCKET)
	{
		WSACleanup();
		throw std::runtime_error("Failed to create socket.");
	}
	
	// Reserve port for socket connection
	myAddress.sin_family = AF_INET;
	
	// Use local IP.
	inet_pton(AF_INET, "0.0.0.0", &myAddress.sin_addr);

	myAddress.sin_port = htons(PORT);
	// Bind to local socket.

	if (bind(mySocket, (SOCKADDR*)&myAddress, sizeof(myAddress)) == SOCKET_ERROR)
	{
		WSACleanup();
		throw std::runtime_error("Failed to bind socket.");
	}

	int server_length = sizeof(struct sockaddr_in);
	int result;

	std::vector<char> vec(MAX_SIZE);
	
	std::cout << "finished." << std::endl;

	while (true)
	{
		result = recvfrom(mySocket, vec.data(), MAX_SIZE, 0, (SOCKADDR*)&otherAddress, &server_length);
		if (result != -1) {
			vec.resize(result);

			std::cout << "Received packet." << std::endl;

			DisplayBuffer::add(vec);

			vec.resize(MAX_SIZE);
		}
	}
}