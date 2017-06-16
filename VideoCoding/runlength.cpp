#include "stdafx.h"
#include "Runlength.h"
#include "Huffman.h"

#include "Decoder\DeRunlength.h"

std::vector<unsigned char> Runlength::run(std::vector<int_fast16_t> in) {
	std::vector<int_fast16_t> out; // Empty vector
	out.reserve(img_res_cbcr); // Reserve a convervative amount of data in the vector. (Here 1/3 the full size)

	size_t j = 0;
	// For counting the amount of zeroes.
	uint_fast16_t zeroCount = 0;

	// Iterates entire input.
	for (const auto &val : in) {
		if (j == 64) { // Is next block
			out.push_back(0);
			out.push_back(0); // 00 for end block.
			zeroCount = 0;
			j = 0;
			out.push_back(val);
		}
		else if (val == 0) {
			++zeroCount;
		} else {
			// If it's not zero add a zero, the amount of zeroes before it, and the number.
			if (zeroCount > 0) {
				out.push_back(0);
				out.push_back(zeroCount);
				zeroCount = 0;
			}
			out.push_back(val);
		}

		++j;
	}

	out.push_back(0);
	out.push_back(0);


	// return DeRunlength::deRun(out);
	return Huffman::huff(out);
}