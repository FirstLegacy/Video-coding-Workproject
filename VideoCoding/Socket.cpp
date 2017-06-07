#include "stdafx.h"
#include "Socket.h"

#include<stdio.h>
#include<winsock2.h>
#include <Ws2tcpip.h>
#include <fstream>
#include <iterator>
#include <iostream>
#include <string>
#include <bitset>
#include <type_traits>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "127.0.0.1"  //ip address of udp server
#define BUFLEN 512  //Max length of buffer
#define PORT 8890   //The port on which to listen for incoming data
// #define _WINSOCK_DEPRECATED_NO_WARNINGS

SOCKET s = 0;
int slen = 0;
char UDP_header = '00';
struct sockaddr_in si_other;

void Socket::SendFrame(std::vector<char> message) {
	// fill
	int packetSize = 5;
	int splitSize = message.size / packetSize;
	int i = 0;
	for (i = 1; i <= splitSize; i++); {
		//Socket::send(split_lo(message.begin()*i, message.begin() + packetSize*i));
		std::vector<char> split_lo(message.begin() + packetSize*i, message.begin() + (packetSize * i) + packetSize);
		

		char aChar = UDP_header + i;
		std::vector<char> header = { aChar };
		
		header.insert(header.end(), split_lo.begin(), split_lo.end());
		Socket::send(header);
	}

}

// SFINAE for safety. Sue me for putting it in a macro for brevity on the function
#define IS_INTEGRAL(T) typename std::enable_if< std::is_integral<T>::value >::type* = 0

template<class T>
std::string integral_to_binary_string(T byte, IS_INTEGRAL(T))
{
	std::bitset<sizeof(T) * CHAR_BIT> bs(byte);
	return bs.to_string();
}

int main() {
}

// The function to send a message through the socket
void Socket::send(std::vector<char> message) {
	if (sendto(s, &message.data()[0], (int)message.size(), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
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

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//setup address structure
	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	inet_pton(AF_INET, SERVER, &si_other.sin_addr);
	// si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

	// Socket is now ready to start communication
}

void Socket::closeConnection()
{
	closesocket(s);
	WSACleanup();
}

// // // // //
// Beyond is un-used code for extensions or debug
// // // // //

//receive a reply and print it
//clear the buffer by filling null, it might have previously received data
//memset(buf, '\0', BUFLEN);
//try to receive some data, this is a blocking call
//if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR)
//{
//	printf("recvfrom() failed with error code : %d", WSAGetLastError());
//	exit(EXIT_FAILURE);
//}

//puts(buf);

//start communication
//while (1)
//{
//printf("Enter message : ");
//gets(message);

//send the message
//if (sendto(s, message, strlen(message), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
//{
//	printf("sendto() failed with error code : %d", WSAGetLastError());
//	exit(EXIT_FAILURE);
//}

//receive a reply and print it
//clear the buffer by filling null, it might have previously received data
//memset(buf, '\0', BUFLEN);
//try to receive some data, this is a blocking call
//if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR)
//{
//	printf("recvfrom() failed with error code : %d", WSAGetLastError());
//	exit(EXIT_FAILURE);
//}

//puts(buf);
//}

//closesocket(s);
//WSACleanup();

//return 0;