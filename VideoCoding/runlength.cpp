#include "stdafx.h"
#include "Runlength.h"
#include "Huffman.h"

std::vector<char> Runlength::run(std::vector<int_fast16_t> in) {
	std::vector<int_fast16_t> out; // Empty vector
	out.reserve(img_res_ycbcr / 4); // Reserve a convervative amount of data in the vector.

	size_t j = 0;
	uint_fast16_t zeroCount = 0;

	for (const auto &val : in) {
		if (j == 64) { // Is next block
			out.push_back(0);
			out.push_back(0); // 00 for end block.
			zeroCount = 0;
			j = 0;
		}
		if (val == 0) {
			zeroCount++;
		} else {
			if (zeroCount > 0) {
				out.push_back(0);
				out.push_back(zeroCount);
				zeroCount = 0;
			}
			out.push_back(val);
		}

		j++;
	}

	return Huffman::huff(out);
}