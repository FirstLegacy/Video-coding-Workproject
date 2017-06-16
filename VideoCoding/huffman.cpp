#include "stdafx.h"
#include "Huffman.h"

#include "Decoder\DeHuffman.h"

#define EOB 11 // End of Block is entry 11 in the AC Array

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
const std::array<bitvec, 12> Huffman::acTable = {							// Value:	Total size:
	bitvec{ false, false },													// Next is a length of zeroes.
	bitvec{ false, true },													// 1			3
	bitvec{ true, false },													// 2			4
	bitvec{ true, true, true, false },										// 3			7
	bitvec{ true, true, true, true, false },								// 4			8
	bitvec{ true, true, true, true, true, false },							// 5			11
	bitvec{ true, true, true, true, true, true, false },					// 6			13
	bitvec{ true, true, true, true, true, true, true, false },				// 7			15
	bitvec{ true, true, true, true, true, true, true, true, false },		// 8			17
	bitvec{ true, true, true, true, true, true, true, true, true, false },	// 9			18
	bitvec{ true, true, true, true, true, true, true, true, true, true },	// 10			20
	bitvec{ true, true, false }												// EOB			6
};

// Huffman table for a length of zeroes.
const std::array<bitvec, 6> Huffman::zeroLengthTable = {	// Value:				Size(this)	Size(total)
	bitvec{ false },										// One 0					1			3
	bitvec{ true, false },									// 1						2			5
	bitvec{ true, true, false },							// 2						3			7
	bitvec{ true, true, true, false },						// 3						4			9
	bitvec{ true, true, true, true, false },				// 4						5			11
	bitvec{ true, true, true, true, true },					// 5						5			12
};

// Calculate the signed value table - at compile time (doesn't work, runs on startup) - from here...
// Gets correct values.
constexpr std::bitset<11> f(int_fast16_t n) {
	return n < 0 ? std::bitset<11>(-n).flip() : std::bitset<11>(n); // Gets binary representation of values.
																    // Flips bits if negative.
}

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

// Calculate the zero value table - at compile time (doesn't work, runs on startup) - from here...
// Gets correct values.
template <size_t val>
constexpr typename std::enable_if<(val > 31), std::bitset<5>>::type
g() {
	return std::bitset<5>(val - 32);
}

template <size_t val>
constexpr typename std::enable_if<(val > 15 && val <= 31), std::bitset<5>>::type
g() {
	return std::bitset<5>(val - 16);
}

template <size_t val>
constexpr typename std::enable_if<(val > 7 && val <= 15), std::bitset<5>>::type
g() {
	return std::bitset<5>(val - 8);
}

template <size_t val>
constexpr typename std::enable_if<(val > 3 && val <= 7), std::bitset<5>>::type
g() {
	return std::bitset<5>(val - 4);
}

template <size_t val>
constexpr typename std::enable_if<(val <= 3), std::bitset<5>>::type
g() {
	return std::bitset<5>(val - 2);
}

// If only one input start with the lowest value (-2047)
// N is max size.
template <int N>
constexpr typename std::vector<std::bitset<5>>
makeZero() {
	return makeZero<N, 3>(std::vector<std::bitset<5>>{g<2>()}); // First value is 2
}

// Returns if size reaches N.
template <int N, size_t size>
constexpr typename std::enable_if<N == size, std::vector<std::bitset<5>>>::type
makeZero(std::vector<std::bitset<5>> Vals) {
	return Vals;
}

// Recursive function adding to vector.
// This function is called if the size has not yet reached N, adding another value.
template <int N, size_t size>
constexpr typename std::enable_if<N != size, std::vector<std::bitset<5>>>::type
makeZero(std::vector<std::bitset<5>> Vals) {
	Vals.push_back(g<size>());
	return makeZero<N, size + 1>(Vals);
}

const std::vector<std::bitset<5>> Huffman::zeroValueTable = makeZero<64>();

// 2 to the power of 1-10 for easy lookup
const std::array<int_fast16_t, 12> Huffman::two_pow = {
	1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048
};

// Inserts bits from bitvec
void Huffman::insertBits(std::vector<char> &out, bitvec bits, uint_fast8_t &reached) {
	uint_fast8_t reached_buffer = reached;
	
	for (size_t i = 0; i < bits.size(); ++i) {
		if (reached_buffer == 0) {
			out.push_back(0);
		}

		out.back() |= (bits.at(i) << reached_buffer);
		
		if (++reached_buffer == CHAR_BIT) {
			reached_buffer = 0;
		}
	}

	reached = reached_buffer;
}

// Inserts bits from bitset
template <size_t size>
void Huffman::insertBits(std::vector<char> &out, std::bitset<size> bits,
						 size_t length, uint_fast8_t &reached) {
	
	uint_fast8_t reached_buffer = reached;

	for (size_t i = length - 1; i != -1; --i) {
		if (reached_buffer == 0) {
			out.push_back(0);
		}

		out.back() |= (bits.test(i) << reached_buffer);

		if (++reached_buffer == CHAR_BIT) {
			reached_buffer = 0;
		}
	}

	reached = reached_buffer;
}

// Inserts the length in bit of the value table.
void Huffman::insertLength(std::vector<char> &out, size_t length, int_fast8_t type, uint_fast8_t &reached) {
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
		bits = zeroLengthTable.at(length);
		break;
	default: // AC table
		bits = acTable.at(length);
		break;
	}
	insertBits(out, bits, reached);
}

// Inserts the value.
void Huffman::insertSignedValue(std::vector<char> &out, size_t length,
								int_fast16_t val, uint_fast8_t &reached) {
	auto bits = signValueTable.at(val + 2047);
	insertBits(out, bits, length, reached);
}

void Huffman::insertZeroValue(std::vector<char> &out, size_t length,
							  int_fast16_t val, uint_fast8_t &reached) {
	auto bits = zeroValueTable.at(val - 2);
	insertBits(out, bits, length, reached);
}

// Handles inserting the bits.
void Huffman::inserter(std::vector<char> &out, int_fast16_t current,
					   int_fast16_t last, int_fast8_t type, uint_fast8_t &reached) {
	if (last == 0 && type == 1) { // If it's a length of zeroes.
		if (current == 0) {
			insertLength(out, EOB, 1, reached); // End of Block
		}
		else {
			insertLength(out, 0, 1, reached); // Insert zero-run indicator
			if (current == 1) {
				insertLength(out, 0, -2, reached); // Insert length of 1 (indicating a single zero).
			}
			else {
				for (size_t i = 1; i < 6; ++i) {
					if (current < two_pow.at(i + 2)) {
						insertLength(out, i, -2, reached); // Insert length of value.
						insertZeroValue(out, i, current, reached); // Insert value if the length is not 1.
						break;
					}
				}
			}
		}
	}
	else if (current != 0 || type != 1) {
		for (size_t i = 1; i < two_pow.size(); ++i) {
			if (abs(current) < two_pow.at(i)) {
				insertLength(out, i, type, reached); // Insert length of value.
				insertSignedValue(out, i, current, reached); // Insert actual value.
				break;
			}
		}
	}
}

// Huffman encoder.
// Returns char-array, which is what socket can send.
std::vector<unsigned char> Huffman::huff(std::vector<int_fast16_t> in) {
	uint_fast8_t reached = 0;
	std::vector<char> out;
	out.reserve(8000); // Allocate 8 KB (It doesn't matter if it's too high).

	int_fast8_t dcmeasure = 0; // Lum DC if 0, Chrom DC if -1 else AC
	size_t dc_count = 0;

	uint_fast16_t last = -1;

	for (const auto &current : in) {
		Huffman::inserter(out, current, last, dcmeasure, reached);

		if (current == 0 && last == 0 && dcmeasure == 1) {
			// Once the dc count exceeds the amount of DC values in the luminance parts the chroma part is being processed.
			if (dc_count == img_y_dc_values) {
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

	// return out;
	return DeHuffman::huff(out);
}
