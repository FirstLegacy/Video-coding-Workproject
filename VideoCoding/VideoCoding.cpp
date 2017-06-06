#include "stdafx.h"

#include <vector>
#include <Windows.h>
#include <thread>
#include <iostream>

#include "rgbToYCbCr.h"
#include "Camera.h"
#include "quantize.h"
#include "Socket.h"
#include "Decoder\Interface.h"

#define MS_PER_FRAME 1000/24 // ms / fps = ms per frame

unsigned int frameCount = 0;

void send_frames(std::vector<unsigned char> image) {
	std::vector<char> coded_image = RgbToYCbCr::convert(image);
	// Socket::send(coded_image);
	
	Interface::frame = image;
}

int main() {
	// Camera::test();
	
	Camera::startCam();
	Quantize::setQuality(1);
	std::vector<unsigned char> image = Camera::getFrame();

	std::thread gui(Interface::GUI, image);

	// Socket::connect();
	time_t start, end;
	double diff;

	while (true) {
		time(&start);

		image = Camera::getFrame();

		std::thread sf(send_frames, image);

		sf.detach();
		
		time(&end);

		diff = difftime(start, end);

		Sleep((DWORD)(MS_PER_FRAME - diff));
	}

	gui.join();
	
}
