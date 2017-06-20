#include "stdafx.h"
#include "rgbToYCbCr.h"
#include "Camera.h"
#include "quantize.h"
#include "Socket.h"

#include <thread>
#include <string>
#include <iostream>
#include <atomic>

#define MS_PER_FRAME 1000/24 // ms / fps = ms per frame

std::atomic_size_t running_threads = 0;

void sendFrames() {
	std::vector<char> coded_image = RgbToYCbCr::convert(Camera::getFrame());

	Socket::sendFrame(coded_image);

	--running_threads;
}

int main(int argc, char* argv[]) {
	std::cout << "Starting video encoder." << std::endl << std::endl;
	// Arg 1 is quality
	// Arg 2 is IP
	// Arg 3 is port

	// IMPORTANT: Quality in encoder and decoder MUST be the same.

	char* ip = "127.0.0.1";
	int port = 22205;
	char* quality = "default";

	if (argc > 1) {
		ip = argv[1];
	}
	if (argc > 2) {
		quality = argv[2];
		Quantize::setQuality(std::stoi(argv[2]));
	}
	if (argc > 3) {
		port = std::stoi(argv[3]);
	}

	std::cout << "Target IP: " << ip << std::endl;
	std::cout << "Port: " << port << std::endl;
	std::cout << "Quality: " << quality << std::endl;


	auto suppThreads = std::thread::hardware_concurrency();

	Camera::startCam();

	Socket::connect(port, ip);
	
	while (true) {
		if (running_threads < suppThreads) { // Wait until there's not more running threads than cores.
			++running_threads;
			std::thread sf(sendFrames);
			sf.detach();

			std::this_thread::sleep_for(std::chrono::milliseconds(MS_PER_FRAME));
		}
	}

	Socket::closeConnection();
}
