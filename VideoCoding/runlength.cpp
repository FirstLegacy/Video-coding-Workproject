#include "stdafx.h"
#include "Runlength.h"

#include <vector>
#include <array>

std::vector<char> Runlength::run(std::array<uint_fast16_t, img_res_ycbcr> in) {
	std::vector<uint_fast16_t> out; // Empty vector

	for (const auto &val : in) {
		// Do run-length stuff.
	}
}