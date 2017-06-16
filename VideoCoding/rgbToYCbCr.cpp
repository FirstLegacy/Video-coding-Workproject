#include "stdafx.h"
#include "RgbToYCbCr.h"
#include "ToBlock.h"

std::vector<char> RgbToYCbCr::convert(std::vector<unsigned char> in) {
// Inputtet er i unsigned char er 8 bit som kommer i sæt af 3 for hver pixel
// Total size: image_res_rgb (width * height * 3). With default settings 921600

	std::vector<uint8_t> yArray(img_res);
	std::vector<uint8_t> cbArray(img_res);
	std::vector<uint8_t> crArray(img_res);
	std::vector<uint8_t> out_putArray(img_res_ycbcr);

	int R;
	int G;
	int B;

	//http://www.equasys.de/colorconversion.html Det er her fra at formlen er taget fra til RBG til YCbCr

	for (size_t i = 0; i < img_res; ++i) {
		R = in.at(i * 3 + 2); // opencv uses BGR not RGB
		G = in.at(i * 3 + 1);
		B = in.at(i * 3);

		yArray.at(i)	= (uint8_t)( 0.299 * R + 0.587 * G + 0.114 * B);
		cbArray.at(i)	= (uint8_t)(-0.169 * R - 0.331 * G + 0.500 * B + 128);
		crArray.at(i)	= (uint8_t)( 0.500 * R - 0.419 * G - 0.081 * B + 128);
	}

	downSampling(yArray, cbArray, crArray, &out_putArray);
	

	return ToBlock::blockify(out_putArray);
}

void RgbToYCbCr::downSampling(
			std::vector<uint8_t> yArray,
			std::vector<uint8_t> cbArray,
			std::vector<uint8_t> crArray,
			std::vector<uint8_t> *out_putArray) {
	
	std::vector<uint8_t> out_cbArray(img_res / 4);
	std::vector<uint8_t> out_crArray(img_res / 4);

	size_t j = 0;

	for (size_t i = 0; i < img_res;) {

		out_cbArray.at(j) = cbArray.at(i);
		out_crArray.at(j) = crArray.at(i);

		i += 2;
		++j;
		if (i % img_res_w == 0) { // || i % img_res_w == 1){
			i += img_res_w;
		}
	}

	size_t i = 0;

	for (const auto &val : yArray) {
		out_putArray->at(i) = val;
		++i;
	}

	for (const auto &val : out_cbArray) {
		out_putArray->at(i) = val;
		++i;
	}

	for (const auto &val : out_crArray) {
		out_putArray->at(i) = val;
		++i;
	}
}
