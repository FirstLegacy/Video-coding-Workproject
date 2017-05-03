#include "stdafx.h"
#include "RgbToYCbCr.h"

#include <vector>
#include <array>



std::vector<char> RgbToYCbCr::convert(std::array<uint8_t,  in_res> in) {
// inputtet er i unit8_t som er 8 bit som kommer i sæt af 3 for hver pixel
//
	int R;
	int G;
	int B;
	std::array<uint8_t, in_res> yArray;
	std::array<uint8_t, in_res> cbArray;
	std::array<uint8_t, in_res> crArray;

	int i = 0;
	for(const auto &val : in){
		R = val[0];
		G = val[1];
		B = val[2];

		yArray[i]	= (77 / 256)*R + (150 / 256)*G + (29 / 256)*B;
		cbArray[i]	= -(44 / 256)*R -(87 / 256)*G + (131 / 256)*B + 128;
		crArray[i]	= (131 / 256)*R -(110 / 256)*G -(21 / 256)*B + 128;
		i++;
	}
}

void RgbToYCbCr::downSampling(std::array<uint8_t, in_res_ycbcr> &in) {



}
