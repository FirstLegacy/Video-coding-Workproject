#include "stdafx.h"
#include "Socket.h"

#include <Ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib") // Winsock library.

#define ID 0x0F // Header ID byte (00001111).
#define MAX_SIZE 1472
#define MAX_FRAME_SIZE MAX_SIZE - 7 // Max size of a payload.

uint32_t frame_count = 0;

SOCKET mySocket;
sockaddr_in otherAddress;
int slen;

void Socket::sendFrame(std::vector<char> message) {
	uint8_t frame_part_amount = (uint8_t)ceil((float)message.size() / MAX_FRAME_SIZE);

	for (uint8_t frame_part_count = 0; frame_part_count < frame_part_amount; ++frame_part_count) {
		auto start = message.begin() + MAX_FRAME_SIZE * frame_part_count;

		std::vector<char> split_lo(start,
				(frame_part_count == frame_part_amount - 1) ? message.end() : (start + MAX_FRAME_SIZE));
		
		char fc[4];

		fc[0] = (frame_count >> 24) & 0xFF;
		fc[1] = (frame_count >> 16) & 0xFF;
		fc[2] = (frame_count >> 8) & 0xFF;
		fc[3] = frame_count & 0xFF;

		std::vector<char> header = { ID, fc[0], fc[1], fc[2], fc[3],
					(char)frame_part_count, (char)frame_part_amount };

		header.insert(header.end(), split_lo.begin(), split_lo.end());
		Socket::sendMsg(header);
	}
	++frame_count;
}

// The function to send a message through the UDP socket.
void Socket::sendMsg(std::vector<char> message) {
	if (sendto(mySocket, message.data(), (int)message.size(), 0, (struct sockaddr *) &otherAddress, slen) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

// Initialize socket with port and IP-address
void Socket::connect(int port, char *server)
{
	WSADATA wsaData;

	slen = sizeof(otherAddress);

	//Initialise Winsock
	std::cout << "Socket initializing . . . ";
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		WSACleanup();
		throw std::runtime_error("Failed to startup WSA.");
	}

	// Create socket
	if ((mySocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		WSACleanup();
		throw std::runtime_error("Failed to create socket.");
	}

	// Setup address structure.
	// Reserve port for socket connection
	otherAddress.sin_family = PF_INET;

	// Listen from any IP (i.e. no matter what IP the program is on).
	inet_pton(PF_INET, server, &otherAddress.sin_addr);

	// Set port.
	otherAddress.sin_port = htons(port);

	std::cout << "finished." << std::endl;
}

void Socket::closeConnection()
{
	closesocket(mySocket);
	WSACleanup();
}
