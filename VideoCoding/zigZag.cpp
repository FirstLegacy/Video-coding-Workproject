#include "stdafx.h"
#include "ZigZag.h"
#include "Runlength.h"

const std::array<uint_fast8_t, mBlockSize> zigzagOrder =
{
	 0,  1,  8, 16,  9,  2,  3, 10,
	17, 24, 32, 25, 18, 11,  4,  5,
	12, 19, 26, 33, 40, 48, 41, 34,
	27, 20, 13,  6,  7, 14, 21, 28,
	35, 42, 49, 56, 57, 50, 43, 36,
	29, 22, 15, 23, 30, 37, 44, 51,
	58, 59, 52, 45, 38, 31, 39, 46,
	53, 60, 61, 54, 47, 55, 62, 63
};

std::vector<char> ZigZag::zigzag(std::array<uint_fast16_t, img_res_ycbcr> in) {
	std::array<uint_fast16_t, img_res_ycbcr> out;

	for (size_t i = 0; i < img_div_blocksize; i += mBlockSize) {
		for (size_t j = 0; j < mBlockSize; ++j) {
			out.at(i + zigzagOrder.at(j)) = in.at(i + j);
		}
	}

	return Runlength::run(out);
}