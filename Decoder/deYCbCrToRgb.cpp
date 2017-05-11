#include "stdafx.h"
#include "deYCbCrToRgb.h"

#include <vector>
#include <array>

int in_size = img_res / 4;
std::array<uint8_t, img_res/4> in_yArray;
std::array<uint8_t, img_res/4> in_cbArray;
std::array<uint8_t, img_res/4> in_crArray;

std::array<uint8_t, img_res> yArray;
std::array<uint8_t, img_res> cbArray;
std::array<uint8_t, img_res> crArray;

std::array<uint8_t, img_res_rbg> out_rbgArray;

void deYCbCrToRgb::upSampling(std::array<uint8_t, (img_res * 3) / 4 > &in) {
	int i = 0;

	for (const auto &val : in) {
		if (in_yArray.size() <= i) {
			in_yArray[i] = in[i];
		}
		else if (in_yArray.size() + in_cbArray.size() <= i) {
			in_cbArray[i- in_size] = in[i];
		}
		else if (in_yArray.size() + in_cbArray.size() + in_crArray.size() <= i) {
			in_crArray[i - (in_size*2)] = in[i];
		}
	}

	int i1 = 0;
	int i2 = 0;
	for (const auto &val : in_yArray) {
		if (i2 % img_res == 0) {
			i2 = i2 + img_res;
		}
		in_yArray[i1] = yArray[i2];
		in_yArray[i1] = yArray[i2 + 1];
		in_yArray[i1] = yArray[i2 + img_res];
		in_yArray[i1] = yArray[i2 + img_res + 1];

		in_cbArray[i1] = cbArray[i2];
		in_cbArray[i1] = cbArray[i2 + 1];
		in_cbArray[i1] = cbArray[i2 + img_res];
		in_cbArray[i1] = cbArray[i2 + img_res + 1];

		in_crArray[i1] = crArray[i2];
		in_crArray[i1] = crArray[i2 + 1];
		in_crArray[i1] = crArray[i2 + img_res];
		in_crArray[i1] = crArray[i2 + img_res + 1];

		i1++; 
		i2 = i2 + 2;
	}
}


deYCbCrToRgb::deYCbCrToRgb()
{
	
	int Y;
	int Cb;
	int Cr;
	int i = 0;
	for (const auto &val : yArray) {
		Y	= yArray[0];
		Cb	= cbArray[0];
		Cr	= crArray[0];

		out_rbgArray[i]		=	1.000*Y + 0.000*(Cb-128) + 1.400*(Cr-128);
		out_rbgArray[i+1]	= 	1.000*Y - 0.343*(Cb-128) - 0.711*(Cr-128);
		out_rbgArray[i+2]	=	1.000*Y + 1.765*(Cb-128) - 0.000*(Cr-128);
		
		i = i + 3;
	}

}