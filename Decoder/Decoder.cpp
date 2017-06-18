#include "stdafx.h"
#include "SocketServer.h"
#include "DisplayBuffer.h"
#include "Interface.h"
#include "DeQuantize.h"

#include <thread>
#include <string>
#include <iostream>

#define MS_PER_FRAME 1000/24

int main(int argc, char* argv[]) {
	std::cout << "Starting video decoder." << std::endl << std::endl;

	int deleteFromBufferAfter = 10000; // MS_PER_FRAME;
	int port = 22205;

	char* quality = "default";

	if (argc > 1) {
		quality = argv[1];
		DeQuantize::setQuality(std::stoi(argv[1]));
	}
	if (argc > 2) {
		port = std::stoi(argv[2]);
	}
	if (argc > 3) {
		deleteFromBufferAfter = std::stoi(argv[3]);
	}

	std::cout << "Quality: " << quality << std::endl;
	std::cout << "Keep unfinished frame in buffer for: " << deleteFromBufferAfter << "ms" << std::endl;
	std::cout << "Port: " << port << std::endl;

	DisplayBuffer::bufferDeleteTime = deleteFromBufferAfter;

	Interface::init();

	std::thread listen(SocketServer::listen, port);

	std::thread handler(DisplayBuffer::mergeHandler);

	std::vector<unsigned char> image;
	bool gotImage;
	
	while (true) {
		gotImage = DisplayBuffer::get(image);

		if (gotImage) {
			Interface::update(image);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(MS_PER_FRAME));
	}

	listen.join();
	handler.join();
}
