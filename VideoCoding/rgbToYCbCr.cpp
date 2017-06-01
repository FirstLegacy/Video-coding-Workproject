#include "stdafx.h"
#include "RgbToYCbCr.h"
#include "ToBlock.h"

std::array<uint8_t, img_res> yArray;
std::array<uint8_t, img_res> cbArray;
std::array<uint8_t, img_res> crArray;
std::array<uint8_t, img_res_ycbcr> out_putArray;

std::vector<char> RgbToYCbCr::convert(std::array<uint8_t, img_res_rgb> in) {
// inputtet er i unit8_t som er 8 bit som kommer i sæt af 3 for hver pixel
//
	int R;
	int G;
	int B;

	//http://www.equasys.de/colorconversion.html Det er her fra at formlen er taget fra til RBG til YCbCr
	int i = 0;
	for(const auto &val : in){
		R = in[0];
		G = in[1];
		B = in[2];

		yArray[i]	=	0.299*R	+0.587*G + 0.114*B;
		cbArray[i]	=  -0.169*R	-0.331*G + 0.500*B + 128;
		crArray[i]	=	0.500*R	-0.419*G - 0.081*B + 128;
		i++;
	}

	// Skal også bruge downsampling et eller andet sted her.

	return ToBlock::blockify(out_putArray);
}

void RgbToYCbCr::downSampling(std::array<uint8_t, img_res_rgb> &in) {
	std::array<uint8_t, img_res/4> out_yArray;
	std::array<uint8_t, img_res/4> out_cbArray;
	std::array<uint8_t, img_res/4> out_crArray;


	int i = 0;
	for (const auto &val : in) {
		out_yArray[i] = yArray[i];
		out_cbArray[i] = cbArray[i];
		out_crArray[i] = crArray[i];
		if(i % img_res_w == 0 || i % img_res_w == 1){
			i = i + img_res_w + 1;
		}
		else {
		i = i + 2;
		}
	}
	for (const auto &val : in) {
		if (out_yArray.size() <= i) {
			out_putArray[i] = out_yArray[i];
		}
		else if (out_yArray.size() + out_cbArray.size() <= i) {
			out_putArray[i] = out_cbArray[i];
		}
		else if (out_yArray.size() + out_cbArray.size() + out_crArray.size() <= i) {
			out_putArray[i] = out_crArray[i];
		}
	}
}
