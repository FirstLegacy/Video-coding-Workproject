#include "stdafx.h"
#include "deToBlock.h"
#include "deYCbCrToRgb.h"


std::vector<unsigned char> deToBlock::deBlockify(std::vector<uint_fast8_t> block) {
	std::vector<uint_fast8_t> out(img_res_ycbcr);

	size_t tempWidth = img_res_w;
	static size_t y_block_count = img_res / mBlockSize;

	for (size_t i = 0; i < img_block_count; ++i) { // Iterate blocks
		for (size_t j = 0; j < blockSize; ++j) { // Iterate y-axis
			for (size_t k = 0; k < blockSize; ++k) { // Iterate x-axis
				if (i == y_block_count) {
					tempWidth = img_res_w / 2;
				}
				out.at(mBlockSize * i + blockSize * j + k) = block.at(blockSize * i + tempWidth * j + k);
				// Assigns to the block.
			}
		}
	}

	return deYCbCrToRgb::upSampling(out);
}
