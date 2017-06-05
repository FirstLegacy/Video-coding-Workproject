#include "stdafx.h"
#include "Quantize.h"
#include "ZigZag.h"


// https://www.w3.org/Graphics/JPEG/itu-t81.pdf Page 143
// JPEG standard quantization tables.
// Luminance Quantization Table
const std::array<int_fast16_t, 64> Quantize::lumTableOrig =
{ 16, 11, 10, 16, 24,  40,  51,  61,
  12, 12, 14, 19, 26,  58,  60,  55,
  14, 13, 16, 24, 40,  57,  69,  56,
  14, 17, 22, 29, 51,  87,  80,  62,
  18, 22, 37, 56, 68,  109, 103, 77,
  24, 35, 55, 64, 81,  104, 113, 92,
  49, 64, 78, 87, 103, 121, 120, 101,
  72, 92, 95, 98, 112, 100, 103, 99 };

// Chrominance Quantization Table
const std::array<int_fast16_t, 64> Quantize::chromTableOrig =
{ 17, 18, 24, 47, 99, 99, 99, 99,
  18, 21, 26, 66, 99, 99, 99, 99,
  24, 26, 56, 99, 99, 99, 99, 99,
  46, 66, 99, 99, 99, 99, 99, 99,
  99, 99, 99, 99, 99, 99, 99, 99,
  99, 99, 99, 99, 99, 99, 99, 99,
  99, 99, 99, 99, 99, 99, 99, 99,
  99, 99, 99, 99, 99, 99, 99, 99 };

// Non-const versions for changing quality.
std::array<int_fast16_t, 64> Quantize::lumTable = lumTableOrig;
std::array<int_fast16_t, 64> Quantize::chromTable = chromTableOrig;

std::vector<char> Quantize::quant(std::vector<int_fast16_t> in) {
	size_t j = 0;
	// First quantize the luminance part using the lum table
	for (size_t i = 0; i < img_res; ++i) {
		// J is used to find the correct part of the table.
		if (j == 64)
			j = 0;

		in.at(i) /= lumTable.at(j);

		++j;
	}
	
	// Quantize the chrom table.
	for (size_t i = img_res; i < img_res_ycbcr; ++i) {
		if (j == 64)
			j = 0;

		in.at(i) /= chromTable.at(j);

		++j;
	}

	return ZigZag::zigzag(in);
}

// Sets quality
// 100 is max quality, 1 is min quality
void Quantize::setQuality(int_fast16_t q) {
	int_fast16_t s;
	q < 50 ? s = 5000 / q : s = 200 - q * 2;

	for (size_t i = 0; i < mBlockSize; ++i) {
		lumTable.at(i) = lumTableOrig.at(i) * (s + 50) / 100;
		chromTable.at(i) = chromTableOrig.at(i) * (s + 50) / 100;
	}
}