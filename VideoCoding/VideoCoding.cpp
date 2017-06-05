#include "stdafx.h"

#include <vector>

#include "rgbToYCbCr.h"
#include "Camera.h"
#include "quantize.h"

unsigned int frameCount = 0;

int main() {
	// Camera::test();
	
	Camera::startCam();

	std::vector<unsigned char> image = Camera::getFrame();

	Quantize::setQuality(1);

	auto coded_img = RgbToYCbCr::convert(image);

	auto conv = coded_img;
}
