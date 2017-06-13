#include "stdafx.h"
#include "deToBlock.h"
#include "deYCbCrToRgb.h"


std::vector<unsigned char> deToBlock::deBlockify(std::vector<uint_fast8_t> in) {
	std::vector<uint_fast8_t> block(img_res_ycbcr);

	for (size_t i = 0; i < img_block_count; ++i) { // Iterate blocks
		for (size_t j = 0; j < blockSize; ++j) { // Iterate y-axis
			for (size_t k = 0; k < blockSize; ++k) { // Iterate x-axis
				block.at(blockSize * i + img_res_w * j + k) = in.at(mBlockSize * i + blockSize * j + k);
				// Assigns to the block.
			}
		}
	}

	return deYCbCrToRgb::upSampling(block);
}
