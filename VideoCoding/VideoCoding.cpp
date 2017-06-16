#include "stdafx.h"
#include "rgbToYCbCr.h"
#include "Camera.h"
#include "quantize.h"
#include "Socket.h"

#include <thread>
//#include <iostream>

#define MS_PER_FRAME 1000/24 // ms / fps = ms per frame

size_t running_threads = 0;

void sendFrames() {
	auto image = Camera::getFrame();

	std::vector<char> coded_image = RgbToYCbCr::convert(image);

	Socket::sendFrame(coded_image);

	--running_threads;
}

int main() {
	Quantize::setQuality(25);

	Socket::connect();
	
	Camera::startCam();

	volatile auto suppThreads = std::thread::hardware_concurrency();
	// Volatile prevents the empty while-loop below from being optimized away.

	while (true) {
		while (running_threads >= suppThreads) { } // Wait until there's not more running threads than cores.
		++running_threads;
		std::thread sf(sendFrames);
		sf.detach();
		
		std::this_thread::sleep_for(std::chrono::milliseconds(MS_PER_FRAME));
	}

	Socket::closeConnection();
}
