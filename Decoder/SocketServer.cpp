#include "stdafx.h"
#include "SocketServer.h"
#include "DisplayBuffer.h"

#include <iostream>
#include <Ws2tcpip.h>
#include <thread>

#pragma comment(lib, "Ws2_32.lib") // Winsock library.

#define MAX_SIZE 1460

// void init(int port)
void SocketServer::listen(int port)
{
	std::cout << "Socket initializing . . . ";

	WSADATA wsaData;
	SOCKET mySocket;
	sockaddr_in myAddress, otherAddress;

	// Create socket
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		WSACleanup();
		throw std::runtime_error("Failed to startup WSA.");
	}

	if ((mySocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		WSACleanup();
		throw std::runtime_error("Failed to create socket.");
	}
	
	// Reserve port for socket connection
	myAddress.sin_family = PF_INET;

	// Listen from any IP (i.e. no matter what IP the program is on).
	inet_pton(PF_INET, "0.0.0.0", &myAddress.sin_addr);

	// Set port.
	myAddress.sin_port = htons(port);

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
		if (result == SOCKET_ERROR) {
			std::cout << "Socket recvfrom error: " << WSAGetLastError() << std::endl;
		}
		if (result > 0) {
			vec.resize(result);

			DisplayBuffer::add(vec);

			vec.resize(MAX_SIZE);
		}
	}
}
