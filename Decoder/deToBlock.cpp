#include "deToBlock.h"

#include <vector>
#include <array>

uint8_t* deBlockify(uint8_t*** blocks) {

	int size = 8;
	uint8_t* listBytes = new uint8_t[size];
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			for (int k = 0; k < size; ++k)
				listBytes[size*size*i + size*j + k] = blocks[i][j][k];

	return listBytes;
}
