#include "stdafx.h"
#include "DeRunlength.h"
#include "DeZigZag.h"


std::vector<unsigned char> DeRunlength::deRun(std::vector<int_fast16_t> in)
{
	std::vector<int_fast16_t> out;
	out.reserve(img_res_ycbcr); // Allocate the correct size.

	size_t j = 0;

	int_fast16_t lastVal = -1;

	// Iterates entire input.
	for (const auto &val : in) {
		if (val == 0) {
			; // Do nothing
		}
		else if (lastVal == 0) { // Insert run of zeroes
			for (size_t i = 0; i < val; ++i) {
				out.push_back(0);
			}
		}
		else {
			out.push_back(val);
		}

		lastVal = val;
	}

	return DeZigZag::unzigzag(out);
}
