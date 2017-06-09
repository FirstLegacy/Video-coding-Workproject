#include "stdafx.h"
#include "deToBlock.h"
#include "deYCbCrToRgb.h"

#include <vector>
#include <array>

/*
std::vector<uint_fast8_t> deToBlock::deBlockify(std::vector<char> blocks) {
	std::vector<uint_fast8_t> aList; // Empty stack.
	// TODO
	// Remove temp integers
	int temp_img_res = 9;
	int temp_mBlockSize = 8;
	aList.reserve(temp_img_res);
	std::vector<uint_fast8_t> listBytes;
	for (int i = 0; i < temp_mBlockSize; ++i)
		for (int j = 0; j < temp_mBlockSize; ++j)
			for (int k = 0; k < temp_mBlockSize; ++k)
				aList.at(temp_mBlockSize * i + temp_mBlockSize * j + k) = blocks.at(i);

	return aList;
}
*/

	std::vector<unsigned char> deToBlock::deBlockify(std::vector<uint_fast8_t> in) {
		std::vector<uint_fast8_t> block(img_res_ycbcr);

		size_t tempWidth = img_res_w;

		for (size_t i = 0; i < img_block_count; ++i) { // Iterate blocks
			for (size_t j = 0; j < blockSize; ++j) { // Iterate x-axis
				for (size_t k = 0; k < blockSize; ++k) { // Iterate y-axis
					if (mBlockSize * i + blockSize * j + k < img_res) {
						tempWidth = img_res_w / 2;
					}
					block.at(blockSize * i + tempWidth * j + k) = in.at(mBlockSize * i + blockSize * j + k);
					// Assigns to the block.
				}
			}
		}

		return deYCbCrToRgb::upSampling(block);
	}