#include "stdafx.h"
#include "DeHuffman.h"
#include "DeRunlength.h"

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
const std::array<bitvec, 6> DeHuffman::zeroLengthTable = {	// Value:				Size(this)	Size(total)
	bitvec{ false },											// One 0					1			3
	bitvec{ true, false },									// 1						2			5
	bitvec{ true, true, false },							// 2						3			7
	bitvec{ true, true, true, false },						// 3						4			9
	bitvec{ true, true, true, true, false },				// 4						5			11
	bitvec{ true, true, true, true, true },					// 5						5			12
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

template<typename T, size_t size>
size_t checkLength(std::array<T, size> arr, bitvec in) {
	for (size_t i = 0; i < size; ++i) {
		if (arr.at(i) == in) {
			return i; // return index
		}
	}

	return -1; // -1 for not found.
}

// Inserts the length in bit of the value table.
int_fast16_t DeHuffman::getLength(bitvec in, int_fast8_t type) {
	size_t len = -1;
	switch (type)
	{
	case 0: // Lum DC table
		len = checkLength(dcLumTable, in);
		break;
	case -1: // Chroma DC table
		len = checkLength(dcChromTable, in);
		break;
	case -2:
		len = checkLength(zeroLengthTable, in);
		break;
	default: // AC table
		len = checkLength(acTable, in);
		break;
	}

	return (int_fast16_t)len;
}

// Inserts the bits of the value from signed table.
int_fast16_t DeHuffman::getValue(std::vector<char> in, size_t length,
								 size_t start_byte, size_t start_bit) {
	std::bitset<11> bits{ 0 };
	size_t j = 0;
	for (size_t i = 0; i < length; ++i, ++j) {
		if (start_bit + i == 8) { // 8th bit is invalid, set to bit 0 for next byte.
			++start_byte;
			start_bit = 0;
			j = 0;
		}
		bits[length - i] = (in.at(start_byte) >> (start_bit + j)) & 1;
	}
	
	for (size_t i = two_pow.at(length - 1); i < two_pow.at(length); ++i) {
		if (signValueTable.at(2047 + i) == bits) {
			return (int_fast16_t)i;
		}
		else if (signValueTable.at(2047 - i) == bits) {
			return -(int_fast16_t)i;
		}
	}

	throw std::runtime_error("Value not found in signValueTable.");
}

// Inserts the bits of the value fron zero table.
int_fast16_t DeHuffman::getZeroValue(std::vector<char> in, size_t length,
									 size_t start_byte, size_t start_bit) {
	std::bitset<5> bits{ 0 };
	size_t j = 0;
	for (size_t i = 0; i < length; ++i, ++j) {
		if (start_bit + i == 8) { // 8th bit is invalid, set to bit 0 for next byte.
			++start_byte;
			start_bit = 0;
			j = 0;
		}
		bits[length - i] = (in.at(start_byte) >> (start_bit + j)) & 1;
	}

	for (size_t i = two_pow.at(length); i < two_pow.at(length + 1); ++i) {
		if (zeroValueTable.at(i) == bits) {
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
	static const size_t y_dc_values = img_res / mBlockSize; // The amount of DC values in the luminance part.
	size_t dc_count = 0;
	size_t num_count = 0;

	int_fast8_t len = -1;
	bool eob_check = false;

	bool current;
	
	for (size_t i = 0; i < in.size(); ++i) {
		for (size_t j = 0; j < CHAR_BIT; ++j) {
			current = (in.at(i) >> j) & 1; // Current bit
			buffer.push_back(current);

			len = getLength(buffer, dcmeasure);

			if (len == -1) { // If nothing was found.
				; // Do nothing
			}
			else { // If a value is found.
				if (dcmeasure == -2) { // If it's a length of zeroes.
					if (len == 0) {
						out.push_back(0); // If len is 0, next value is a single zero.
						eob_check = true;
					}
					else {
						out.push_back(getZeroValue(in, len, i, j + 1));
					}
					dcmeasure = 1;
				}
				else if (len == 0 && dcmeasure == 1) { // If next part is a length of zeroes.
					if (eob_check) {
						out.push_back(0); // Adds second zero, signifying EOB.
						dcmeasure = 0; // Next is DC
						eob_check = false;

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
						dcmeasure = -2;
					}
				}
				else if (dcmeasure == 1) { // If AC.
					out.push_back(getValue(in, len, i, j + 1));
					eob_check = false;
				}
				else { // If DC
					if (len == 0) {
						out.push_back(0);
					}
					else {
						out.push_back(getValue(in, len, i, j + 1));
					}
					dcmeasure = 1;
				}

				j += len;
				if (j > 8) {
					i += j / 8;
					j += j % 8;
				}
			}
		}
	}
	
	return DeRunlength::deRun(out);
}
