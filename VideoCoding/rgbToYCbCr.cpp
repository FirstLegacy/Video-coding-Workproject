#include "stdafx.h"
#include "RgbToYCbCr.h"
#include "ToBlock.h"

std::vector<char> RgbToYCbCr::convert(std::array<uint8_t, img_res_rgb> in) {
// inputtet er i unit8_t som er 8 bit som kommer i sæt af 3 for hver pixel
//

	std::vector<uint8_t> yArray;
	std::vector<uint8_t> cbArray;
	std::vector<uint8_t> crArray;
	std::vector<uint8_t> out_putArray;

	int R;
	int G;
	int B;

	//http://www.equasys.de/colorconversion.html Det er her fra at formlen er taget fra til RBG til YCbCr
	int i = 0;
	for(const auto &val : in){
		R = in[0];
		G = in[1];
		B = in[2];

		yArray.at(i)	=	0.299*R	+0.587*G + 0.114*B;
		cbArray.at(i)	=  -0.169*R	-0.331*G + 0.500*B + 128;
		crArray.at(i)	=	0.500*R	-0.419*G - 0.081*B + 128;
		i++;
	}

	// Skal også bruge downsampling et eller andet sted her.
	downSampling(yArray, cbArray, crArray, &out_putArray);

	return ToBlock::blockify(out_putArray);
}

void RgbToYCbCr::downSampling(
			std::vector<uint8_t> yArray,
			std::vector<uint8_t> cbArray,
			std::vector<uint8_t> crArray,
			std::vector<uint8_t> *out_putArray) {
	
	std::vector<uint8_t> out_yArray;
	std::vector<uint8_t> out_cbArray;
	std::vector<uint8_t> out_crArray;


	int i = 0;
	for (size_t i = 0; i < img_res_rgb;) {
		out_yArray.at(i) = yArray.at(i);
		out_cbArray.at(i) = cbArray.at(i);
		out_crArray.at(i) = crArray.at(i);
		if(i % img_res_w == 0 || i % img_res_w == 1){
			i = i + img_res_w + 1;
		}
		else {
		i = i + 2;
		}
	}
	for (size_t i = 0; i < img_res_ycbcr; ++i) {
		if (out_yArray.size() >= i) {
			out_putArray->at(i) = out_yArray.at(i);
		}
		else if (out_yArray.size() + out_cbArray.size() >= i) {
			out_putArray->at(i) = out_cbArray.at(i);
		}
		else if (out_yArray.size() + out_cbArray.size() + out_crArray.size() >= i) {
			out_putArray->at(i) = out_crArray.at(i);
		}
	}
}
