#include "stdafx.h"
#include "ToBlock.h"
#include "DCT.h"
#include "Decoder\DeToBlock.h"

std::vector<char> ToBlock::blockify(std::vector<uint_fast8_t> in) {
	std::vector<uint_fast8_t> block(img_res_ycbcr);

	static size_t w_blocks = img_res_w / blockSize;
	static size_t h_blocks = img_res_h / blockSize;
	
	for (size_t h = 0; h < h_blocks; ++h) {
		for (size_t i = 0; i < w_blocks; ++i) { // Iterate blocks
			for (size_t j = 0; j < blockSize; ++j) { // Iterate x-axis
				for (size_t k = 0; k < blockSize; ++k) { // Iterate y-axis
					block.at(mBlockSize * (i + w_blocks * h) + blockSize * j + k)
						= in.at(img_res_w * blockSize * h + blockSize * i + img_res_w * j + k);
					// Assigns to the block.
				}
			}
		}
	}

	static size_t cbcr_width = img_res_w / 2;
	static size_t cbcr_h_blocks = h_blocks;
	static size_t cbcr_w_blocks = w_blocks / 2;
	
	for (size_t h = 0; h < cbcr_h_blocks; ++h) { // Iterate blocks vertically
		for (size_t i = 0; i < cbcr_w_blocks; ++i) { // Iterate blocks horizontally
			for (size_t j = 0; j < blockSize; ++j) { // Iterate x-axis
				for (size_t k = 0; k < blockSize; ++k) { // Iterate y-axis
					block.at(img_res + mBlockSize * (i + cbcr_w_blocks * h) + blockSize * j + k)
						= in.at(img_res + cbcr_width * blockSize * h + blockSize * i + cbcr_width * j + k);
					// Assigns to the block.
				}
			}
		}
	}

	return DCT::transform(block);
	// return deToBlock::deBlockify(block);
}
