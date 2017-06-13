#include "stdafx.h"
#include "DeHuffman.h"
#include "DeRunlength.h"

#define EOB 3

// JPEG standard DC Chroma Table
const std::unordered_map<bitvec, int_fast16_t> DeHuffman::dcChromMap {// Value:	Total size:
	{ bitvec{ false, true },										0 },	//	2
	{ bitvec{ false, false },										1 },	//	3
	{ bitvec{ true, false, false },									2 },	//	5
	{ bitvec{ true, false, true },									3 },	//	6
	{ bitvec{ true, true, false, false },							4 },	//	8
	{ bitvec{ true, true, false, true },							5 },	//	9
	{ bitvec{ true, true, true, false },							6 },	//	10
	{ bitvec{ true, true, true, true, false },						7 },	//	12
	{ bitvec{ true, true, true, true, true, false },				8 },	// 	14
	{ bitvec{ true, true, true, true, true, true, false },			9 },	// 	16
	{ bitvec{ true, true, true, true, true, true, true, false },   10 },	//	18
	{ bitvec{ true, true, true, true, true, true, true, true },	   11 }		//	19
};

// JPEG standard DC Luminance Table
const std::unordered_map<bitvec, int_fast16_t> DeHuffman::dcLumMap {
	{ bitvec{ true, true, false },							0 },
	{ bitvec{ true, false, true },							1 },
	{ bitvec{ false, true, true },							2 },
	{ bitvec{ false, true, false },							3 },
	{ bitvec{ false, false, false },						4 },
	{ bitvec{ false, false, true },							5 },
	{ bitvec{ true, false, false },							6 },
	{ bitvec{ true, true, true, false },					7 },
	{ bitvec{ true, true, true, true, false },				8 },
	{ bitvec{ true, true, true, true, true, false },		9 },
	{ bitvec{ true, true, true, true, true, true, false },	10},
	{ bitvec{ true, true, true, true, true, true, true },	11}
};

// Custom AC table for both Luminance and Chroma. It might change.
const std::unordered_map<bitvec, int_fast16_t> DeHuffman::acMap = {
	{ bitvec{ false, false },														0 },
	{ bitvec{ false, true },														1 },
	{ bitvec{ true, false },														2 },
	{ bitvec{ true, true, false },													EOB },
	{ bitvec{ true, true, true, false },											3 },
	{ bitvec{ true, true, true, true, false },										4 },
	{ bitvec{ true, true, true, true, true, false },								5 },
	{ bitvec{ true, true, true, true, true, true, false },							6 },
	{ bitvec{ true, true, true, true, true, true, true, false },					7 },
	{ bitvec{ true, true, true, true, true, true, true, true, true, false },		8 },
	{ bitvec{ true, true, true, true, true, true, true, true, true, true, false },	9 },
	{ bitvec{ true, true, true, true, true, true, true, true, true, true, true },	9 }
};

// Huffman table for a length of zeroes.
const std::unordered_map<bitvec, int_fast16_t> DeHuffman::zeroLengthMap = {
	{ bitvec{ false },								0 },
	{ bitvec{ true, false },						1 },
	{ bitvec{ true, true, false },					2 },
	{ bitvec{ true, true, true, false },			3 },
	{ bitvec{ true, true, true, true, false },		4 },
	{ bitvec{ true, true, true, true, true },		5 }
};

//____________________________________________________________________________________________________________

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
const std::vector<std::bitset<11>> DeHuffman::signValueTable =
makeVal<4096, 3584>( // 4095 is the amount of values, but one more since it actually skips the last value
makeVal<3584, 3072>(
makeVal<3072, 2560>(
makeVal<2560, 2048>(
makeVal<2048, 1536>(
makeVal<1536, 1024>(
makeVal<1024, 512>(
makeVal<512>()))))))); // Split up due to recursive function limits in the Visual C++ compiler.

//__________________________________________________________________________________________________________


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

// If only one input start with the lowest value (2)
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

const std::vector<std::bitset<5>> DeHuffman::zeroValueTable = makeZero<64>();

//______________________________________________________________________________________________________

// 2 to the power of 1-10 for easy lookup
const std::array<int_fast16_t, 12> DeHuffman::two_pow = {
	1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048
};

// Inserts the length in bit of the value table.
int_fast16_t DeHuffman::getLength(bitvec in, int_fast8_t type) {
	switch (type)
	{
	case 0: // Lum DC table
		if (dcLumMap.find(in) != dcLumMap.end()) {
			return dcLumMap.at(in);
		}
		return -1;
	case -1: // Chroma DC table
		if (dcChromMap.find(in) != dcChromMap.end()) {
			return dcChromMap.at(in);
		}
		return -1;
	case -2:
		if (zeroLengthMap.find(in) != zeroLengthMap.end()) {
			return zeroLengthMap.at(in);
		}
		return -1;
	default: // AC table
		if (acMap.find(in) != acMap.end()) {
			return acMap.at(in);
		}
		return -1;
	}
}

// Inserts the bits of the value from signed table.
int_fast16_t DeHuffman::getValue(std::vector<char> in, size_t length,
	size_t &start_byte, size_t &start_bit) {

	std::bitset<11> bits{ 0 };
	size_t m_length = length - 1;

	for (size_t i = 0; i < length; ++i, ++start_bit) {
		if (start_bit == 8) { // 8th bit is invalid, set to bit 0 for next byte.
			++start_byte;
			start_bit = 0;
		}
		bits[m_length - i] = ((in.at(start_byte) >> start_bit) & 1);
	}

	bool positive = true;

	if (bits[m_length] == 0) {
		for (size_t i = length; i < 11; ++i) {
			bits.set(i);
		}
		positive = false;
	}

	if (positive) {
		for (size_t i = two_pow.at(m_length); i < two_pow.at(length); ++i) {
			if (signValueTable.at(2047 + i) == bits) {
				return (int_fast16_t)i;
			}
		}
	} else {
		for (size_t i = two_pow.at(m_length); i < two_pow.at(length); ++i) {
			if (signValueTable.at(2047 - i) == bits) {
				return -(int_fast16_t)i;
			}
		}
	}

	throw std::runtime_error("Value not found in signValueTable.");
}

// Gets the value from the zero value table.
int_fast16_t DeHuffman::getZeroValue(std::vector<char> in, size_t length,
									 size_t &start_byte, size_t &start_bit) {
	std::bitset<5> bits{ 0 };
	
	for (size_t i = 0; i < length; ++i, ++start_bit) {
		if (start_bit == 8) { // 8th bit is invalid, set to bit 0 for next byte.
			++start_byte;
			start_bit = 0;
		}
		bits[i] = ((in.at(start_byte) >> start_bit) & 1);
	}

	for (size_t i = two_pow.at(length); i < two_pow.at(length + 1); ++i) {
		if (zeroValueTable.at(i - 2) == bits) {
			return (int_fast16_t)i;
		}
	}

	throw std::runtime_error("Value not found in zeroValueTable.");
}

// Huffman decoder.
std::vector<unsigned char> DeHuffman::huff(std::vector<char> in) {
	std::vector<int_fast16_t> out;
	out.reserve(img_res_cbcr); // Reserve a convervative amount of data in the vector. (Here 1/3 the full size)

	bitvec buffer;

	int_fast8_t dcmeasure = 0; // Lum DC if 0, Chrom DC if -1 else AC

	size_t dc_count = 0;

	int_fast8_t len = -1;

	bool current;
	size_t j_buffer = 0;
	size_t i_buffer = 0;

	for (size_t i = 0; i < in.size(); ++i) {
		for (size_t j = j_buffer; j < CHAR_BIT; ++j) {
			current = (in.at(i) >> j) & 1; // Current bit
			buffer.push_back(current);
			
			len = getLength(buffer, dcmeasure);

			if (len != -1) { // If a value is found.
				j_buffer = j + 1;
				i_buffer = i;
				
				if (dcmeasure == -2) { // If it's a length of zeroes.
					if (len == 0) {
						out.push_back(1); // If len is 0, next value is a single zero.
					}
					else {
						out.push_back(getZeroValue(in, len, i_buffer, j_buffer));
					}
					dcmeasure = 1;
				}
				else if (len == 0 && dcmeasure == 1) { // If next part is a length of zeroes.
					out.push_back(0);
					dcmeasure = -2;
				}
				else if (dcmeasure == 1) { // If AC.
					if (len == EOB) {
						out.push_back(0);
						out.push_back(0); // Adds two zeroes, signifying EOB.

						if (dc_count > img_y_dc_values) {
							dcmeasure = -1;
						}
						else {
							++dc_count;
							dcmeasure = 0;
						}
					}
					else {
						out.push_back(getValue(in, len, i_buffer, j_buffer));
					}
					
				}
				else { // If DC
					if (len == 0) {
						out.push_back(0);
					}
					else {
						out.push_back(getValue(in, len, i_buffer, j_buffer));
					}
					dcmeasure = 1;
				}
				buffer.clear();
				j = j_buffer - 1;

				if (i_buffer > i) {
					i += i_buffer - i;
					break;
				}
			}
			j_buffer = 0;
		}
	}
	
	return DeRunlength::deRun(out);
}
