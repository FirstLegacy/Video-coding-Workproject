#include "stdafx.h"
#include "deYCbCrToRgb.h"

std::vector<unsigned char> deYCbCrToRgb::upSampling(std::vector<uint_fast8_t> in) {
	std::vector<uint8_t> yArray(img_res);
	std::vector<uint8_t> cbArray(img_res);
	std::vector<uint8_t> crArray(img_res);

	size_t i = 0;
	size_t img_res_c = img_res_cbcr / 2;

	for (const auto &val : in) {
		if (i <= img_res) {
			yArray.at(i) = val;
		}
		else if (i <= img_res + img_res_c) {
			cbArray.at(i) = val;
			cbArray.at(i + 1) = val;
			cbArray.at(i + img_res_w) = val;
			cbArray.at(i + img_res_w + 1) = val;
		}
		else {
			crArray.at(i) = val;
			crArray.at(i + 1) = val;
			crArray.at(i + img_res_w) = val;
			crArray.at(i + img_res_w + 1) = val;
		}

		++i;
	}

	return yCbCrToRGB(yArray, cbArray, crArray);
}

std::vector<unsigned char> deYCbCrToRgb::yCbCrToRGB(std::vector<uint_fast8_t> yArray,
						std::vector<uint_fast8_t> cbArray, std::vector<uint_fast8_t> crArray)
{
	int Y;
	int Cb;
	int Cr;

	std::vector<unsigned char> out(img_res_rgb);

	for (size_t i = 0; i < img_res_rgb; i += 3) {
		Y	= yArray.at(i);
		Cb	= cbArray.at(i);
		Cr	= crArray.at(i);

		out.at(i+2)	=	(unsigned char)(1.000*Y + 0.000*(Cb-128) + 1.400*(Cr-128));
		out.at(i+1)	=	(unsigned char)(1.000*Y - 0.343*(Cb-128) - 0.711*(Cr-128));
		out.at(i)	=	(unsigned char)(1.000*Y + 1.765*(Cb-128) - 0.000*(Cr-128));
	}

	return out;
}