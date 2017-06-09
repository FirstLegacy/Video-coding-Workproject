#include "stdafx.h"
#include "SocketServer.h"

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


using namespace std;


//init
int server_length;
int port = 8890;
int STRLEN = 256;
char recMessage[256];
char sendMessage[256];
char *sendMes = "SERVER READY";
WSADATA wsaData;
SOCKET mySocket;
SOCKET myBackup;
SOCKET acceptSocket;
sockaddr_in myAddress;

void init()
{

	//create socket
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		cerr << "Socket Initialization: Error with WSAStartup\n";
		do {
			cout << '\n' << "Press the Enter key to continue.";
		} while (cin.get() != '\n');
		WSACleanup();
		exit(10);
	}

	mySocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (mySocket == INVALID_SOCKET)
	{
		cerr << "Socket Initialization: Error creating socket" << endl;
		do {
			cout << '\n' << "Press the Enter key to continue.";
		} while (cin.get() != '\n');
		WSACleanup();
		exit(11);
	}

	myBackup = mySocket;

	//bind to port
	// Reserve port for socket connection
	myAddress.sin_family = AF_INET;
	// local ip
	myAddress.sin_addr.s_addr = inet_addr("0.0.0.0");
	myAddress.sin_port = htons(port);

	if (bind(mySocket, (SOCKADDR*)&myAddress, sizeof(myAddress)) == SOCKET_ERROR)
	{
		cerr << "ServerSocket: Failed to connect\n";
		do {
			cout << '\n' << "Press the Enter key to continue.";
		} while (cin.get() != '\n');
		WSACleanup();
		exit(14);
	}

	cout << endl;

	char buf[200];
	int fd;
	long recvlen;

	std::vector<char> frameConstruct;
	
	// wait for 
	while (1)
	{
		server_length = sizeof(struct sockaddr_in);
		recvfrom(mySocket, recMessage, STRLEN, 0, (SOCKADDR*)&myAddress, &server_length);
		cout << recMessage << endl;
		sendto(mySocket, sendMes, strlen(sendMes), 0, (SOCKADDR*)&myAddress, server_length);

		//Convert message recieved to vector
		std::vector<char> v(recMessage, recMessage + sizeof recMessage / sizeof recMessage[0]);
		
		/*
		//recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *)&claddr, &clientlen);
		if (recvlen < 0) {
			perror("cannot recvfrom()");
			return;
		}
		printf("Received %ld bytes\n", recvlen);
		buf[recvlen] = 0;
		printf("Received message: \"%s\"\n", buf);
		*/

		frameConstruct.insert(frameConstruct.end(), v.begin(), v.end());
		
		// Forward contructed frame when finished

	}

}