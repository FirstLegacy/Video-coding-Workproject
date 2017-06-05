#include "stdafx.h"
#include "ToBlock.h"
#include "DCT.h"


std::vector<char> ToBlock::blockify(std::vector<uint_fast8_t> in) {
	std::vector<uint_fast8_t> block(img_res_ycbcr);

	size_t tempWidth = img_res_w;

	for (size_t i = 0; i < img_block_count; ++i) { // Iterate blocks
		for (size_t j = 0; j < blockSize; ++j) { // Iterate x-axis
			for (size_t k = 0; k < blockSize; ++k) { // Iterate y-axis
				if (mBlockSize * i + blockSize * j + k < img_res) {
					tempWidth = img_res_w / 2;
				}
				block.at(mBlockSize * i + blockSize * j + k) = in.at(blockSize * i + tempWidth * j + k);
				// Assigns to the block.
			}
		}
	}

	return DCT::transform(block);
}
