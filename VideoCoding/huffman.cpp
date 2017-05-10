#include "stdafx.h"
#include "Huffman.h"

// std::map<uint_fast8_t, bitvec> acLumTable;
// std::map<uint_fast8_t, bitvec> acChromTable;

// JPEG standard
const std::array<bitvec, 12> dcChromTable = {
	bitvec{ false, true },
	bitvec{ false, false },
	bitvec{ true, false, false },
	bitvec{ true, false, true },
	bitvec{ true, true, false, false },
	bitvec{ true, true, false, true },
	bitvec{ true, true, true, false },
	bitvec{ true, true, true, true, false },
	bitvec{ true, true, true, true, true, false },
	bitvec{ true, true, true, true, true, true, false },
	bitvec{ true, true, true, true, true, true, true, false },
	bitvec{ true, true, true, true, true, true, true, true }
};

// JPEG standard
const std::array<bitvec, 12> dcLumTable = {
	bitvec{ true, true, false },
	bitvec{ true, false, true },
	bitvec{ false, true, true },
	bitvec{ false, true, false },
	bitvec{ false, false, false },
	bitvec{ false, false, true },
	bitvec{ true, false, false },
	bitvec{ true, true, true, false },
	bitvec{ true, true, true, true, false },
	bitvec{ true, true, true, true, true, false },
	bitvec{ true, true, true, true, true, true, false },
	bitvec{ true, true, true, true, true, true, true }
};

// Custom
const std::array<bitvec, 11> acTable = {
	bitvec{ false, false },
	bitvec{ false, true, false },
	bitvec{ false, true, true },
	bitvec{ true, false, false },
	bitvec{ true, false, true },
	bitvec{ true, true, false },
	bitvec{ true, true, true, false },
	bitvec{ true, true, true, true, false },
	bitvec{ true, true, true, true, true, false },
	bitvec{ true, true, true, true, true, true, false },
	bitvec{ true, true, true, true, true, true, true }
};

// Calculate value table from here...

// Gets correct values.
constexpr std::bitset<12> f(size_t n) {
	return n < 0 ? std::bitset<12>(n).flip() : std::bitset<12>(n); // Gets binary representation of values.
																   // Flips bits if negative.
}

template <int N>
constexpr typename std::vector<std::bitset<12>>
makeVal() {
	return makeVal<N, 1>(std::vector<std::bitset<12>>{f(0)});
}

// Returns if size reaches N.
template <int N, size_t size>
constexpr
typename std::enable_if<N == size, std::vector<std::bitset<12>>>::type
makeVal(std::vector<std::bitset<12>> Vals) {
	return Vals;
}

// Recursive function adding to vector.
template <int N, size_t size>
constexpr
typename std::enable_if<N != size, std::vector<std::bitset<12>>>::type
makeVal(std::vector<std::bitset<12>> Vals) {
	Vals.push_back(f(Vals.size() - 2047));
	return makeVal<N, size + 1>(Vals);
}
// ...to here.

// Table for bitstrings of signed values. Needs to be calculated.
const auto signValueTable = 
makeVal<4095, 3588>(
makeVal<3587, 3075>(
makeVal<3074, 2562>(
makeVal<2561, 2049>(
makeVal<2048, 1537>(
makeVal<1536, 1025>(
makeVal<1024, 513>(
makeVal<512>()))))))); // Split up due to recursive function limits.

const std::array<uint_fast16_t, 10> two_pow = {
	2, 4, 8, 16, 32, 64, 128, 256, 512, 1024
};

#ifndef FAST16_BITS
#define FAST16_BITS (sizeof(UINT_FAST16_MAX) * CHAR_BIT - 1) // Amount of bits minus 1.
#endif

inline uint_fast16_t Huffman::abs(uint_fast16_t value) {
	uint_fast16_t buffer = value >> FAST16_BITS;     // Make a mask of the sign bit
	value ^= buffer;							// Toggle the bits if value is negative
	return value + buffer & 1;						// Add one if value was negative
}

void Huffman::insertLength(bitvec &out, size_t val, int_fast8_t type) {
	bitvec bits;
	switch (type)
	{
	case 0: // Lum DC table
		bits = dcLumTable[val];
		break;
	case -1: // Chroma DC table
		bits = dcChromTable[val];
		break;
	default: // AC table
		bits = acTable[val];
		break;
	}
	out.insert(out.end(), bits.begin(), bits.end());
}

void Huffman::insertValue(bitvec &out, uint_fast16_t val) {
	auto bits = signValueTable[val];
	out.insert(out.end(), bits.begin(), bits.end());
}

void Huffman::inserter(bitvec &out, uint_fast16_t current, int_fast8_t type) {
	if (current == 0) {
		Huffman::insertLength(out, 0, type); // Insert length of value.
	}
	else {
		for (size_t i = 0; i < two_pow.size(); ++i) {
			if (abs(current) < two_pow[i]) {
				Huffman::insertLength(out, i + 1, type); // Insert length of value.
				Huffman::insertValue(out, current); // Insert actual value.
			}
		}
	}
}

// Huffman encoder.



// Returns char-array, which is what socket can send.
std::vector<char> Huffman::huff(std::vector<uint_fast16_t> in) {
	// HUSK: Længde af char er ikke nødvendigvis 8 bit.
	
	bitvec out;

	int_fast8_t dcmeasure = 0; // Lum DC if 0, Chrom DC if -1 else AC

	for (size_t i = 0; i < img_res; ++i) {
		auto current = in[i];

		Huffman::inserter(out, current, dcmeasure);

		dcmeasure++;

		if (dcmeasure == mBlockSize) {
			dcmeasure = 0;
		}
	}
	
	dcmeasure = -1;

	for (size_t i = img_res; i < img_res_ycbcr; ++i) {
		auto current = in[i];

		Huffman::inserter(out, current, dcmeasure);

		dcmeasure--;

		if (-dcmeasure == mBlockSize + 1) {
			dcmeasure = -1;
		}
	}

	return std::vector<char>{3};
	// return out;
}
