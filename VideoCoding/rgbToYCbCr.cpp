#include "stdafx.h"
#include "RgbToYCbCr.h"

#include <vector>
#include <array>


std::array<uint8_t, img_res> yArray;
std::array<uint8_t, img_res> cbArray;
std::array<uint8_t, img_res> crArray;
std::array<uint8_t, (img_res * 3)> outputArray;

std::vector<char> RgbToYCbCr::convert(std::array<uint8_t, img_res> in) {
// inputtet er i unit8_t som er 8 bit som kommer i sæt af 3 for hver pixel
//
	int R;
	int G;
	int B;


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

void RgbToYCbCr::downSampling(std::array<uint8_t, img_res_ycbcr> &in) {
	std::array<uint8_t, img_res/4> yArrayO;
	std::array<uint8_t, img_res/4> cbArrayO;
	std::array<uint8_t, img_res/4> crArrayO;


	int i = 0;
	for (const auto &val : in) {
		yArrayO[i] = yArray[i];
		cbArrayO[i] = cbArray[i];
		crArrayO[i] = crArray[i];
		if(i % img_res_w == 0 || i % img_res_w == 1){
			i = i + img_res_w + 1;
		}
		else {
		i = i + 2;
		}
	}
	for (const auto &val : in) {
		if (yArrayO.size() <= i) {
			outputArray[i] = yArrayO[i];
		}
		else if (yArrayO.size() + cbArrayO.size() <= i) {
			outputArray[i] = cbArrayO[i];
		}
		else if (yArrayO.size() + cbArrayO.size() + crArrayO.size() <= i) {
			outputArray[i] = crArrayO[i];
		}
	}
}
