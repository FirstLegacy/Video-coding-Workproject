#include "stdafx.h"
#include "ToBlock.h"

#include <vector>
#include <array>

uint8_t*** blockify(uint8_t* bits) {
	int size = 8;
	uint8_t*** block = new uint8_t**[size];
	// for (int i = 0; i < size; ++i)
	//	 block[i][j] = new uint8_t[size];
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			for (int k = 0; k < size; ++k)
				block[i][j][k] = bits[size*size*i + size*j + k];

	return block;

}
