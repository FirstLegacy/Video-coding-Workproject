#include "stdafx.h"

#include <vector>
#include <Windows.h>
#include <thread>

#include "rgbToYCbCr.h"
#include "Camera.h"
#include "quantize.h"
#include "Socket.h"

#define MS_PER_FRAME 1000/24

unsigned int frameCount = 0;

void send_frames() {
	time_t start, end, diff;
	std::vector<unsigned char> image;
	std::vector<char> coded_image;

	while (true) {
		time(&start);

		image = Camera::getFrame();
		coded_image = RgbToYCbCr::convert(image);
		Socket::send(coded_image);

		time(&end);

		diff = difftime(start, end);

		Sleep(MS_PER_FRAME - diff);
	}
}

int main() {
	Camera::test();
	/*
	Camera::startCam();
	Socket::connect();
	Quantize::setQuality(1);

	std::thread sf(send_frames);

	sf.join();*/
}
