#include "stdafx.h"
#include "deYCbCrToRgb.h"

std::vector<unsigned char> deYCbCrToRgb::upSampling(std::vector<uint_fast8_t> in) {
	std::vector<uint8_t> yArray(img_res);
	std::vector<uint8_t> cbArray(img_res);
	std::vector<uint8_t> crArray(img_res);

	size_t i = 0, j = 0, k = 0;
	size_t img_res_c = img_res_cbcr / 2;

	for (const auto &val : in) {
		if (i < img_res) {
			yArray.at(i) = val;
		}
		else if (i < img_res + img_res_c) {
			cbArray.at(j) = val;
			cbArray.at(j + 1) = val;
			cbArray.at(j + img_res_w) = val;
			cbArray.at(j + img_res_w + 1) = val;
			j += 2;
			if (j % img_res_w == 0) {
				j += img_res_w;
			}
		}
		else {
			crArray.at(k) = val;
			crArray.at(k + 1) = val;
			crArray.at(k + img_res_w) = val;
			crArray.at(k + img_res_w + 1) = val;
			k += 2;
			if (k % img_res_w == 0) {
				k += img_res_w;
			}
		}

		++i;
	}

	return yCbCrToRGB(yArray, cbArray, crArray);
}

std::vector<unsigned char> deYCbCrToRgb::yCbCrToRGB(std::vector<uint_fast8_t> yArray,
						std::vector<uint_fast8_t> cbArray, std::vector<uint_fast8_t> crArray)
{
	uint_fast8_t Y;
	uint_fast8_t Cb;
	uint_fast8_t Cr;
	
	std::vector<unsigned char> out(img_res_rgb);
	
	for (size_t i = 0; i < img_res; ++i) {
		Y	= yArray.at(i);
		Cb	= cbArray.at(i);
		Cr	= crArray.at(i);

		/*
		1.000*Y + 0.000*(Cb-128) + 1.400*(Cr-128)
		1.000*Y - 0.343*(Cb-128) - 0.711*(Cr-128)
		1.000*Y + 1.765*(Cb-128) - 0.000*(Cr-128)
		*/

		out.at(i * 3 + 2) = (unsigned char)(Y					   + 1.400 * (Cr - 128));
		out.at(i * 3 + 1) = (unsigned char)(Y - 0.343 * (Cb - 128) - 0.711 * (Cr - 128));
		out.at(i * 3)	  = (unsigned char)(Y + 1.765 * (Cb - 128));
	}

	return out;
}