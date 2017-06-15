#include "stdafx.h"
#include "SocketServer.h"
#include "DisplayBuffer.h"
#include "Interface.h"

#include <thread>
// #include <iostream>

#define MS_PER_FRAME 1000/24

int main(int argc, char* argv[]) {
	std::thread listen(SocketServer::listen);

	Interface::init();
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
}
