#include "stdafx.h"

#include <vector>

#include "rgbToYCbCr.h"
#include "Camera.h"
#include "quantize.h"
#include "socket.h"

int main() {
	//Camera::test();
	
	while (true) {
		frameCount++;

		Camera::startCam();

		std::vector<unsigned char> image = Camera::getFrame();

		Quantize::setQuality(1);

		auto coded_img = RgbToYCbCr::convert(image);

		auto conv = coded_img;

		Socket::connect();

		Socket::send(coded_img);
	}
}
