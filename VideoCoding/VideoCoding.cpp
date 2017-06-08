#include "stdafx.h"

#include <vector>
#include <thread>
#include <iostream>

#include "rgbToYCbCr.h"
#include "Camera.h"
#include "quantize.h"
#include "Socket.h"
#include "Decoder\Interface.h"

#define MS_PER_FRAME 1000/24 // ms / fps = ms per frame

size_t running_threads = 0;

unsigned int frameCount = 0;

void sendFrames() {
	auto image = Camera::getFrame();

	std::vector<char> coded_image = RgbToYCbCr::convert(image);
	volatile char junk = coded_image.at(0);	// To avoid optimizing it away.
	// Socket::send(coded_image);
	
	Interface::updateFrame(image);
	--running_threads;
}

int main() {
	Camera::startCam();
	Quantize::setQuality(1);
	
	std::thread gui(Interface::GUI);

	volatile auto suppThreads = std::thread::hardware_concurrency();
	// Volatile prevents the empty while-loop below from being optimized away.

	// Socket::connect();

	while (true) { // for (size_t i = 0; i < 1000; ++i) { // Only run 1000 times for profiling.
		while (running_threads >= suppThreads) { } // Wait until there's not more running threads than cores.
		++running_threads;
		std::thread sf(sendFrames);
		sf.detach();
		
		std::this_thread::sleep_for(std::chrono::milliseconds(MS_PER_FRAME));
	}

	gui.join();
}
