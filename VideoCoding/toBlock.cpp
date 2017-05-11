#include "stdafx.h"
#include "ToBlock.h"
#include "DCT.h"

// Intet af det her virker, ændrede input og output til det korrekte.
// Block skal være et fladt std::array. new laver den på heap, det er skidt.
// Der bliver kun kørt 8x8x8 hvilket er forkert.
// Desuden er blockSize og mBlockSize globale variabler fra stdafx.h, brug dem.
// Jeg ville nok gøre noget a la
/*
std::vector<char> ToBlock::blockify(std::array<uint_fast8_t, img_res_ycbcr> bits) {
	std::array<uint_fast8_t, img_res_ycbcr> block; // Empty array on stack.
	for (int i = 0; i < img_res_ycbcr / mBlockSize; ++i) { // Iterate blocks
		for (int j = 0; j < blockSize; ++j) { // Iterate x-axis
			for (int k = 0; k < blockSize; ++k) { // Iterate y-axis
				block.at(i) = bits.at(mBlockSize * i + blockSize * j + k); // Det her er det samme som dit, bare anden syntax.
			}
		}
	}

	return DCT::transform(block);
}
*/
// Jeg aner ikke om det virker, men det er rigtig syntax.

std::vector<char> ToBlock::blockify(std::array<uint_fast8_t, img_res_ycbcr> bits) {
	int size = 8;
	uint8_t*** block = new uint8_t**[size];
	// for (int i = 0; i < size; ++i)
	//	 block[i][j] = new uint8_t[size];
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			for (int k = 0; k < size; ++k)
				block[i][j][k] = bits[size*size*i + size*j + k];

	return DCT::transform(block);
}
