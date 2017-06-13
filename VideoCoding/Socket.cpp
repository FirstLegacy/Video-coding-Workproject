#include "stdafx.h"
#include "Socket.h"

#include <Ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib") // Winsock library.

// #define SERVER "169.254.67.196"  // IP adress of the UDP server.
#define SERVER "127.0.0.1"
#define PORT 22205   // Port of the UDP server.
#define MAX_FRAME_SIZE 1453 // Max size of a payload.
#define ID 0x0f // Header ID byte (00001111).
#define MAX_SIZE 1460

SOCKET s = 0;
int slen = 0;
struct sockaddr_in si_other;
uint32_t frame_count = 543099453;

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
	uint8_t frame_part_amount = (uint8_t)ceil((float)message.size() / MAX_FRAME_SIZE);

	for (uint8_t frame_part_count = 0; frame_part_count < frame_part_amount; ++frame_part_count) {
		auto start = message.begin() + MAX_FRAME_SIZE * frame_part_count;

		std::vector<char> split_lo(start,
				(frame_part_count == frame_part_amount - 1) ? message.end() : (start + MAX_FRAME_SIZE));
		
		char *f_c = static_cast<char*>(static_cast<void*>(&frame_count));

		std::vector<char> header = { ID, *f_c, *++f_c, *++f_c, *++f_c,
					(char)frame_part_count, (char)frame_part_amount };

		header.insert(header.end(), split_lo.begin(), split_lo.end());
		Socket::sendMsg(header);
	}
	++frame_count;
}

// The function to send a message through the UDP socket.
void Socket::sendMsg(std::vector<char> message) {

	if (sendto(s, message.data(), (int)message.size(), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	std::vector<char> vec(MAX_SIZE);
	/*
	if (int result = recvfrom(s, vec.data(), MAX_SIZE, 0, (SOCKADDR*)&si_other, &slen) != -1) {
		std::cout << vec.data() << std::endl;
	}
	*/
}

// Initialize socket with port and IP-address
void Socket::connect()
{
	s, slen = sizeof(si_other);
	WSADATA wsa;

	//Initialise Winsock
	std::cout << "Winsock initializing . . . ";
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

	// Setup address structure.
	memset((char *)&si_other, 0, sizeof(si_other)); // Sets all bytes in si_other to 0.
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT); // Assigns port.
	inet_pton(AF_INET, SERVER, &si_other.sin_addr); // Assigns IP-address.

	std::cout << "finished." << std::endl;
}

void Socket::closeConnection()
{
	closesocket(s);
	WSACleanup();
}
