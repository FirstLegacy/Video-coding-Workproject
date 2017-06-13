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
		if (val == 0 && lastVal == 0) {
			std::vector<int_fast16_t> buffer(mBlockSize - j);
			out.insert(out.end(), buffer.begin(), buffer.end());
			j = 0;
		} else if (val == 0) {
			; // Do nothing
		}
		else if (lastVal == 0) { // Insert run of zeroes
			std::vector<int_fast16_t> buffer(val);
			out.insert(out.end(), buffer.begin(), buffer.end());
			j += val;
		}
		else {
			out.push_back(val);
			++j;
		}

		lastVal = val;
	}

	return DeZigZag::unzigzag(out);
}
