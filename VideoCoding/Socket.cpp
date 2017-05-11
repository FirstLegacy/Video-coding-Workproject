#include "stdafx.h"
#include "Socket.h"

#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "127.0.0.1"  //ip address of udp server
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

int s = 0, slen = 0;
struct sockaddr_in si_other;

// The function to send a message through the socket
void Socket::send(std::vector<char> message) {
	if (sendto(s, &message.data()[0], message.size(), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

// Establish socket connection and attempt to connect to defined server
void Socket::connect()
{
	s, slen = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
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
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);
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