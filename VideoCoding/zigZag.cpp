#include "stdafx.h"
#include "ZigZag.h"
#include "Runlength.h"

// Array for the order of the zigzag pattern.
const std::array<uint_fast8_t, mBlockSize> ZigZag::zigzagOrder =
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

// Sets the image in zigzag pattern.
std::vector<unsigned char> ZigZag::zigzag(std::vector<int_fast16_t> in) {
	std::vector<int_fast16_t> out(img_res_ycbcr);

	// Goes through every block and inserts the value in the new vector at the correct place by looking up the table.
	for (size_t i = 0; i < img_block_count; ++i) {
		for (size_t j = 0; j < mBlockSize; ++j) {
			out.at(i * mBlockSize + j) = in.at(i * mBlockSize + zigzagOrder.at(j));
		}
	}

	// return DeZigZag::unzigzag(out);
	return Runlength::run(out);
}