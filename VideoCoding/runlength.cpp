#include "stdafx.h"
#include "Runlength.h"
#include "Huffman.h"

#include <vector>
#include <array>

std::vector<char> Runlength::run(std::array<uint_fast16_t, img_res_ycbcr> in) {
	std::vector<uint_fast16_t> out; // Empty vector

	for (const auto &val : in) {
		size_t j = 0;

		uint_fast16_t zeroCount = 0;
		if (j >= 64) {
			out.push_back(0);
			out.push_back(0);
			zeroCount = 0;
			j = 0;
		} else if (val == 0) {
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