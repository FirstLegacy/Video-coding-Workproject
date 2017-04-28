#include "stdafx.h"
#include "DCT.h"

#include <vector>
#include <array>

// https://pdfs.semanticscholar.org/e024/bdc2b5b6db2d0eed65ca96ae575b600fa3a9.pdf

std::vector<char> DCT::transform(std::array<uint_fast8_t, img_res_ycbcr> in) {
	// HUSK: DCT skal være 16 bit dvs. brug uint_fast16_t


}

// Binary DCT.
// Horizontal if dir is true else vertical.
void DCT::binDCT(std::array<uint_fast8_t, mBlockSize> &arr, bool dir) { 
	//
	// HUSK TIL RAPPORT: Der var fejl i artiklens proposed pipeline, og matricen er blevet brugt.
	//

	// To do: Skal køre for hver række/kolonne af arrayet og ikke kun på starten.
	int
		x0 = arr[0],
		x1 = arr[1],
		x2 = arr[2],
		x3 = arr[3],
		x4 = arr[4],
		x5 = arr[5],
		x6 = arr[6],
		x7 = arr[7];

	// Stage 1:
	int
		a0 = x0 + x7,
		a1 = x1 + x6,
		a2 = x2 + x5,
		a3 = x3 + x4,
		a4 = x3 - x4,
		a5 = x2 - x5,
		a6 = x1 - x6,
		a7 = x1 - x7;

	// Stage 2:
	int
		// b0 = a0,
		// b1 = a1,
		// b2 = a2,
		// b3 = a3,
		// b4 = a4,
		// b7 = a7,
		b5 = a6 * 5 / 8 - a5 * 49 / 64,
		b6 = a5 * 3 / 8 + a6;
	// OR:
	/*
	b6 = a6 + (a5 + (a5 << 1)) >> 3,
	b5 = (b6 + (b6 << 2)) >> 3 - a5;
	*/

	// Stage 3:
	int
		c0 = a0 + a3, // c0 = b0 + b3,
		c1 = a1 + a2, // c1 = b1 + b2,
		c2 = a1 - a2, // c2 = b1 - b2,
		c3 = a0 - a3, // c3 = b0 - b3,
		c4 = a4 + b5, // c4 = b4 + b5,
		c5 = a4 - b5, // c5 = b4 - b5,
		c6 = a7 - b6, // c6 = b7 - b6,
		c7 = b6 + a7; // c7 = b6 + b7;

	// Stage 4:
	int
		d0 = c0 + c1,
		d1 = (c0 - c1) / 2,
		d2 = c2 - c3 * 3 / 8,
		d3 = c2 * 3 / 8 + c3 * 55 / 64,
		d4 = c4 - c7 / 8,
		d5 = c5 + c6 * 7 / 8,
		d6 = c6 * 9 / 16 - c5 / 2;
	// d7 = c7;
	// OR:
	/*
	d0 = c0 + c1,
	d1 = (c0 - 1) / 2,
	d2 = c2 - (c3 + (c3 << 1)) >> 3,
	d3 = (d2 + (d2 << 1)) >> 3 - c3,
	d4 = c4 - c7 / 8,
	d5 = c5 + ((c6 << 3) - c6) >> 3,
	d6 = c6 - (d5 >> 1);
	*/

	// Stage 5:
	/*
	int
		e0 = d0,
		e1 = c7, // e1 = d7,
		e2 = d3,
		e3 = d6,
		e4 = d1,
		e5 = d5,
		e6 = d2,
		e7 = d4;
	*/

	arr[0] = d0;
	arr[1] = c7;
	arr[2] = d3;
	arr[3] = d6;
	arr[4] = d1;
	arr[5] = d5;
	arr[6] = d2;
	arr[7] = d4;
}