//#include "stdafx.h"
#include "deToBlock.h"
#include "DeDCT.h"

#include <vector>
#include <array>

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