#include "stdafx.h"
#include "DCT.h"
#include "Quantize.h"
#include <iostream>
#include "Camera.h"

unsigned int frameCount = 0;

int main() {
	Camera::test();
	
	/*
	Quantize::setQuality(1);
	//for (size_t i = 0; i < 100; ++i) { // Test 100 times
		std::vector<uint_fast8_t> testdata(img_res_ycbcr); // Random vector of correct size

		for (auto &val : testdata) {
			val = rand() % 256;
		}

		std::vector<char> test = DCT::transform(testdata);

		std::cout << test.size() << std::endl;
	//}*/
}
