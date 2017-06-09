#include "stdafx.h"
#include "Socket.h"

#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
// #include <iterator>
#include <iostream>

#pragma comment(lib,"ws2_32.lib") // Winsock library.

#define SERVER "127.0.0.1"  // IP adress of the UDP server.
#define PORT 8890   // Port of the UDP server.

SOCKET s = 0;
int slen = 0;
char ID = 0x0F;
struct sockaddr_in si_other;
uint32_t count = 0;

/*
UDP
IN	PORT:
OUT PORT:
IP		:
Header: 00
Max Packet size: 1460 byte
Used Packet size 1400 byte
*/

void Socket::sendFrame(std::vector<char> message) {
	// fill
	uint16_t frame_part_size = 1400;
	uint8_t frame_part_amount = (uint8_t)ceil((float)message.size() / frame_part_size);

	for (uint8_t frame_part_count = 0; frame_part_count < frame_part_amount; ++frame_part_count) {
		auto start = message.begin() + frame_part_size * frame_part_count;

		std::vector<char> split_lo(start,
				(frame_part_count == frame_part_amount - 1) ? message.end() : (start + frame_part_size));
		
		char *f_c = static_cast<char*>(static_cast<void*>(&frame_count));

		std::vector<char> header = { ID, *f_c, *++f_c, *++f_c, *++f_c,
					(char)frame_part_count, (char)frame_part_amount };

		header.insert(header.end(), split_lo.begin(), split_lo.end());
		Socket::sendMsg(header);
	}
	++frame_count;
}

// The function to send a message through the socket
void Socket::sendMsg(std::vector<char> message) {
	// char *msg = message.data();
	if (sendto(s, message.data(), (int)message.size(), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

// Establish socket connection and attempt to connect to defined server
void Socket::connect()
{
	s, slen = sizeof(si_other);
	WSADATA wsa;

	//Initialise Winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	// Create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code: %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	// Setup address structure
	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	inet_pton(AF_INET, SERVER, &si_other.sin_addr);

	std::cout << "Initialised." << std::endl;
}

void Socket::closeConnection()
{
	closesocket(s);
	WSACleanup();
}
