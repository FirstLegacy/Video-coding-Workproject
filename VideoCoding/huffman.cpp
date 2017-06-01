#include "stdafx.h"
#include "Huffman.h"

// JPEG standard DC Chroma Table
const std::array<bitvec, 12> Huffman::dcChromTable = {			// Value:	Total size:
	bitvec{ false, true },										// 0			2
	bitvec{ false, false },										// 1			3
	bitvec{ true, false, false },								// 2			5
	bitvec{ true, false, true },								// 3			6
	bitvec{ true, true, false, false },							// 4			8
	bitvec{ true, true, false, true },							// 5			9
	bitvec{ true, true, true, false },							// 6			10
	bitvec{ true, true, true, true, false },					// 7			12
	bitvec{ true, true, true, true, true, false },				// 8			14
	bitvec{ true, true, true, true, true, true, false },		// 9			16
	bitvec{ true, true, true, true, true, true, true, false },	// 10			18
	bitvec{ true, true, true, true, true, true, true, true }	// 11			19
};

// JPEG standard DC Luminance Table
const std::array<bitvec, 12> Huffman::dcLumTable = {		// Value:	Total size:
	bitvec{ true, true, false },							// 0			3
	bitvec{ true, false, true },							// 1			4
	bitvec{ false, true, true },							// 2			5
	bitvec{ false, true, false },							// 3			6
	bitvec{ false, false, false },							// 4			7
	bitvec{ false, false, true },							// 5			8
	bitvec{ true, false, false },							// 6			9
	bitvec{ true, true, true, false },						// 7			11
	bitvec{ true, true, true, true, false },				// 8			13
	bitvec{ true, true, true, true, true, false },			// 9			15
	bitvec{ true, true, true, true, true, true, false },	// 10			17
	bitvec{ true, true, true, true, true, true, true }		// 11			18
};

// Custom AC table for both Luminance and Chroma. It might change.
const std::array<bitvec, 11> Huffman::acTable = {						// Value:	Total size:
	bitvec{ false, false },			// Next is a length of zeroes.
	bitvec{ false, true },												// 1			3
	bitvec{ true, false },												// 2			4
	bitvec{ true, true, false },										// 3			6
	bitvec{ true, true, true, false },									// 4			8
	bitvec{ true, true, true, true, false },							// 5			10
	bitvec{ true, true, true, true, true, false },						// 6			12
	bitvec{ true, true, true, true, true, true, false },				// 7			14
	bitvec{ true, true, true, true, true, true, true, false },			// 8			16
	bitvec{ true, true, true, true, true, true, true, true, false },	// 9			18
	bitvec{ true, true, true, true, true, true, true, true, true, true }// 10			19
};


// Huffman table for a length of zeroes.
const std::array<bitvec, 10> Huffman::zeroLengthTable = {	// Value:				Total size (incl. 00 in front):
	bitvec{ false, false, false },							// EOB								5
	bitvec{ true },											// 1								3
	// false, true, true is reserved for full 5-bit number giving a size of 8 bit.				10
	bitvec{ false, true, false },							// 2								5
	bitvec{ false, false, true, true },						// 3								6
	bitvec{ false, false, true, false, false },				// 4								7
	bitvec{ false, false, true, false, true, true },		// 5								8
	bitvec{ false, false, true, false, true, false, true },	// 6								9
	bitvec{ false, false, true, false, true, false, false },// 7								9
};

// Calculate the DC Huffman table - at compile time - from here...
// Gets correct values.
constexpr std::bitset<11> f(int_fast16_t n) {
	return n < 0 ? std::bitset<11>(-n).flip() : std::bitset<11>(n); // Gets binary representation of values.
																    // Flips bits if negative.
}

// Note: Functional programming.
// If only one input start with the lowest value (-2047)
// N is max size.
template <int N>
constexpr typename std::vector<std::bitset<11>>
makeVal() {
	return makeVal<N, 1>(std::vector<std::bitset<11>>{f(-2047)});
}

// Returns if size reaches N.
template <int N, size_t size>
constexpr typename std::enable_if<N == size, std::vector<std::bitset<11>>>::type
makeVal(std::vector<std::bitset<11>> Vals) {
	return Vals;
}

// Recursive function adding to vector.
// This function is called if the size has not yet reached N, adding another value.
template <int N, size_t size>
constexpr typename std::enable_if<N != size, std::vector<std::bitset<11>>>::type
makeVal(std::vector<std::bitset<11>> Vals) {
	Vals.push_back(f((int_fast16_t)Vals.size() - 2047));
	return makeVal<N, size + 1>(Vals);
}
// ...to here.

// Table for bitstrings of signed values. Needs to be calculated.
const std::vector<std::bitset<11>> Huffman::signValueTable =
makeVal<4096, 3584>( // 4095 is the amount of values, but one more since it actually skips the last value
makeVal<3584, 3072>(
makeVal<3072, 2560>(
makeVal<2560, 2048>(
makeVal<2048, 1536>(
makeVal<1536, 1024>(
makeVal<1024, 512>(
makeVal<512>()))))))); // Split up due to recursive function limits in the Visual C++ compiler.

// 2 to the power of 1-10 for easy lookup
const std::array<int_fast16_t, 10> Huffman::two_pow = {
	2, 4, 8, 16, 32, 64, 128, 256, 512, 1024
};

// int_fast16_t is not always 16 bit, so finding out is necessary.
#ifndef FAST16_BITS
#define FAST16_BITS (sizeof(UINT_FAST16_MAX) * CHAR_BIT - 1) // Amount of bits minus 1.
#endif

// Inserts the length in bit of the value table.
void Huffman::insertLength(bitvec &out, size_t length, int_fast8_t type) {
	bitvec bits;
	switch (type)
	{
	case 0: // Lum DC table
		bits = dcLumTable.at(length);
		break;
	case -1: // Chroma DC table
		bits = dcChromTable.at(length);
		break;
	case -2: // Zero length table
		if (length < zeroLengthTable.size()) {
			// If length if 0 then it inserts the EOB value.
			bits = zeroLengthTable.at(length);
		}
		else {
			bits = { false, true, true };
			std::bitset<6> bset = std::bitset<6>(length);
			for (size_t i = 0; i < 6; ++i) {
				bits.push_back(bset[i]);
			}
		}
		break;
	default: // AC table
		bits = acTable.at(length);
		break;
	}
	out.insert(out.end(), bits.begin(), bits.end());
}

// Inserts the bits of the value.
void Huffman::insertValue(bitvec &out, size_t length, int_fast16_t val) {
	auto bits = signValueTable.at(val + 2047);
	for (size_t i = length - 1; i != -1; i--) {
		out.push_back(bits[i]);
	}
}

// Handles inserting the bits.
void Huffman::inserter(bitvec &out, int_fast16_t current, int_fast16_t last, int_fast8_t type) {
	if (current == 0 && last != 0) {
		Huffman::insertLength(out, 0, type); // Insert length of value.
	}
	else if (last == 0) { // If it's a length of zeroes.
		Huffman::insertLength(out, current, -2); // Set the length of a 1 without the value.
	}
	else {
		for (size_t i = 0; i < two_pow.size(); ++i) {
			if (abs(current) < two_pow[i]) {
				Huffman::insertLength(out, i + 1, type); // Insert length of value.
				Huffman::insertValue(out, i + 1, current); // Insert actual value.
				break;
			}
		}
	}
}

// Huffman encoder.
// Returns char-array, which is what socket can send.
std::vector<char> Huffman::huff(std::vector<int_fast16_t> in) {
	// HUSK: L�ngde af char er ikke n�dvendigvis 8 bit.
	
	bitvec bout;
	bout.reserve(3500 * 8); // Reserve 3.5 KB

	int_fast8_t dcmeasure = 0; // Lum DC if 0, Chrom DC if -1 else AC
	const size_t y_dc_values = img_res / mBlockSize; // The amount of DC values in the luminance part.
	size_t dc_count = 0;

	uint_fast16_t last = -1;

	for (const auto &current : in) {
		Huffman::inserter(bout, current, last, dcmeasure);

		if (current == 0 && last == 0) {
			// Once the dc count exceeds the amount of DC values in the luminance parts the chroma part is being processed.
			if (dc_count > y_dc_values) {
				dcmeasure = -1;
			} else {
				++dc_count;
				dcmeasure = 0;
			}
			last = -1; // Last is -1 if next is a new block.
		} else {
			dcmeasure = 1;
			last = current;
		}
	}
		
	// Sets the correct vector, the output HAS to be a char vector.
	std::vector<char> out(bout.size() / CHAR_BIT);

	// Iterates every bit, might not be most efficient.
	for (int i = 0; i < out.size(); ++i) {
		for (int j = 0; j < CHAR_BIT; ++j) {
			out[i] |= bout.at(i * CHAR_BIT + j) << j; // OR bit operation on the bit, it's 0 as default, s� it's set to the bout value.
		}
	}

	return out;
}
