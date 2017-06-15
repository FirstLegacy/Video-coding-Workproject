#include "stdafx.h"
#include "DeRunlength.h"
#include "DeZigZag.h"


std::vector<unsigned char> DeRunlength::deRun(std::vector<int_fast16_t> in)
{
	std::vector<int_fast16_t> out;
	out.reserve(img_res_ycbcr); // Allocate the correct size.

	size_t j = 0;

	bool zero_run = false;
	bool is_dc = true;

	// Iterates entire input.
	for (const auto &val : in) {
		if (zero_run) { // Insert run of zeroes
			if (val == 0) {
				std::vector<int_fast16_t> buffer(mBlockSize - j);
				out.insert(out.end(), buffer.begin(), buffer.end());
				j = 0;
				is_dc = true;
			}
			else {
				std::vector<int_fast16_t> buffer(val);
				out.insert(out.end(), buffer.begin(), buffer.end());
				j += val;
			}
			zero_run = false;
		}
		else if (val == 0 && !is_dc) {
			zero_run = true;
		}
		else {
			out.push_back(val);
			++j;
			zero_run = false;
			is_dc = false;
		}
	}

	return DeZigZag::unzigzag(out);
}
