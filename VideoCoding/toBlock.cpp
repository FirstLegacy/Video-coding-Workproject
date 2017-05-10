#include "stdafx.h"
#include "ToBlock.h"

#include <vector>
#include <array>

uint8_t** blockify(uint8_t* bits) {
	int rows = 8;
	int cols = 8;
	uint8_t** block = new uint8_t*[rows];
	for (int i = 0; i < rows; ++i)
		block[i] = new uint8_t[cols];

	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			block[i][j] = bits[j + rows*i];

	return block;

}
