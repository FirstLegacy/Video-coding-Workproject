#include "stdafx.h"
#include "SocketServer.h"
#include "DeHuffman.h"

#include <vector>
#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <Ws2tcpip.h>
#include <fstream>
#include <iterator>
#include <string>
#include <bitset>
#include <type_traits>

#pragma comment(lib, "Ws2_32.lib") // Winsock library.

#define PORT 8890
#define MAX_SIZE 1460

WSADATA wsaData;
SOCKET mySocket;
sockaddr_in myAddress;

// void init(int port)
void SocketServer::listen()
{
	std::cout << "Socket initializing . . . ";

	// Create socket
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		std::cerr << "Socket Initialization: Error with WSAStartup\n";
		do {
			std::cout << std::endl << "Press the Enter key to continue.";
		} while (std::cin.get() != '\n');
		WSACleanup();
		exit(10);
	}

	mySocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (mySocket == INVALID_SOCKET)
	{
		std::cerr << "Socket Initialization: Error creating socket" << std::endl;
		do {
			std::cout << '\n' << "Press the Enter key to continue.";
		} while (std::cin.get() != '\n');
		WSACleanup();
		exit(11);
	}
	
	// Reserve port for socket connection
	myAddress.sin_family = AF_INET;
	
	// Use local IP.
	inet_pton(AF_INET, "0.0.0.0", &myAddress.sin_addr);

	myAddress.sin_port = htons(PORT);

	// Bind to local socket.
	if (bind(mySocket, (SOCKADDR*)&myAddress, sizeof(myAddress)) == SOCKET_ERROR)
	{
		do {
			std::cerr << "ServerSocket: Failed to connect" << std::endl
				<< "Press any key to continue.";
		} while (std::cin.get() != '\n');
		WSACleanup();
		exit(14);
	}

	int server_length = sizeof(struct sockaddr_in);
	int result;

	std::vector<char> vec(MAX_SIZE);
	
	std::cout << "finished." << std::endl;

	while (true)
	{
		result = recvfrom(mySocket, vec.data(), MAX_SIZE, 0, (SOCKADDR*)&myAddress, &server_length);
		if (result != -1) {
			vec.resize(result);

			// Send vector to buffer control junk.

			vec.resize(MAX_SIZE);
		}
	}
}