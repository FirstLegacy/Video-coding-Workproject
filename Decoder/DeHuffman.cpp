#include "stdafx.h"
#include "DeHuffman.h"

// JPEG standard DC Chroma Table
const std::array<bitvec, 12> DeHuffman::dcChromTable = {		// Value:	Total size:
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
const std::array<bitvec, 12> DeHuffman::dcLumTable = {		// Value:	Total size:
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
const std::array<bitvec, 11> DeHuffman::acTable = {						// Value:	Total size:
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
const std::array<bitvec, 10> DeHuffman::zeroLengthTable = {	// Value:				Total size (incl. 00 in front):
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
constexpr typename std::map<std::bitset<11>, int_fast16_t>
makeVal() {
	return makeVal<N, 0>(std::map<std::bitset<11>, int_fast16_t>{f(-2047), -2047});
}

// Returns if size reaches N.
template <int N, size_t size>
constexpr typename std::enable_if<N == size, std::map<std::bitset<11>, int_fast16_t>>::type
makeVal(std::map<std::bitset<11>, int_fast16_t> Vals) {
	return Vals;
}

// Recursive function adding to vector.
// This function is called if the size has not yet reached N, adding another value.
template <int N, size_t size>
constexpr typename std::enable_if<N != size, std::map<std::bitset<11>, int_fast16_t>>::type
makeVal(std::map<std::bitset<11>, int_fast16_t> Vals) {
	Vals[f((int_fast16_t)Vals.size()) - 2047] = Vals.size() - 2047;
	Vals.push_back(f((int_fast16_t)Vals.size() - 2047));
	return makeVal<N, size + 1>(Vals);
}
// ...to here.

// Table for bitstrings of signed values. Needs to be calculated.
const std::map<std::bitset<11>, int_fast16_t> DeHuffman::signValueMap =
makeVal<4096, 3584>( // 4095 is the amount of values, but one more since it actually skips the last value
	makeVal<3584, 3072>(
		makeVal<3072, 2560>(
			makeVal<2560, 2048>(
				makeVal<2048, 1536>(
					makeVal<1536, 1024>(
						makeVal<1024, 512>(
							makeVal<512>()))))))); // Split up due to recursive function limits in the Visual C++ compiler.

// int_fast16_t is not always 16 bit, so finding out is necessary.
#ifndef FAST16_BITS
#define FAST16_BITS (sizeof(UINT_FAST16_MAX) * CHAR_BIT - 1) // Amount of bits minus 1.
#endif

template<typename T, size_t size>
size_t checkLength(std::array<T, size> arr, bitvec in) {
	for (size_t i = 0; i < size; ++i) {
		if (arr.at(i) == in) {
			return i; // return index
		}
		++i;
	}

	return -1; // -1 for not found.
}

// Inserts the length in bit of the value table.
size_t DeHuffman::getLength(bitvec in, int_fast8_t type) {
	size_t len = -1;
	switch (type)
	{
	case 0: // Lum DC table
		len = checkLength(dcLumTable, in);
		break;
	case -1: // Chroma DC table
		len = checkLength(dcChromTable, in);
		break;
	default: // AC table
		len = checkLength(acTable, in);
		break;
	}

	return len;
}

// Inserts the bits of the value.
int_fast16_t DeHuffman::getValue(std::vector<int_fast16_t> &out, std::vector<char> in, size_t length, size_t val) {
	std::bitset<11> bits;

	for (size_t i = 0; i < length; ++i) {
		bits[length - i] = out.at(val);
	}

	auto sval = signValueMap[bits];

	out.push_back(sval);
}

// Huffman encoder.
// Returns char-array, which is what socket can send.
std::vector<int_fast16_t> DeHuffman::huff(std::vector<char> in) {
	// HUSK: L�ngde af char er ikke n�dvendigvis 8 bit.

	std::vector<int_fast16_t> out;
	out.reserve(img_res_cbcr); // Reserve a convervative amount of data in the vector. (Here 1/3 the full size)

	bitvec buffer;

	int_fast8_t dcmeasure = 1; // Lum DC if 0, Chrom DC if -1 else AC
	const size_t y_dc_values = img_res / mBlockSize; // The amount of DC values in the luminance part.
	size_t dc_count = 0;
	size_t num_count = 0;

	size_t len = -1;

	char current = NULL;
	
	for (size_t i = 0; i < in.size() * CHAR_BIT; ++i) {
		current = in.at(i);
		buffer.push_back(current);

		len = DeHuffman::getLength(buffer, dcmeasure);

		if (len == -1) {
			; // Do nothing
		}
		else if (len == -2) { // Zeroes
			;
		}
		else if (len == 0) {
			out.push_back(0);
			++num_count; // Can only happen on DC values so don't worry about checking AC stuff.
		}
		else {
			num_count += len;

			out.push_back(getVal(out, in, len, i));

			if (num_count == 64) {
				num_count = 0;
				if (dc_count > y_dc_values) {
					dcmeasure = -1;
				}
				else {
					++dc_count;
					dcmeasure = 0;
				}
			}
			else {
				dcmeasure = 1;
			}
		}
	}
	
	return out;
}
