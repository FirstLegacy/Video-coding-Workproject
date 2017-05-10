#include "stdafx.h"
#include "DeHuffman.h"

#include <vector>
#include <map>

// Maybe use bitset instead of vector<bool> depending on ease of implementation. vector<char> might be more efficient.
using bitvec = std::vector<bool>;

std::map<uint_fast8_t, bitvec> dcLumTable;
std::map<uint_fast8_t, bitvec> acLumTable;
std::map<uint_fast8_t, bitvec> dcChromTable;
std::map<uint_fast8_t, bitvec> acChromTable;

// Table for bitstrings of signed values. Needs to be calculated.
std::map<short, bitvec> signValueTable;

// Huffman encoder.
// Returns char-array, which is what socket can send.
std::vector<char> DeHuffman::huff(std::vector<uint_fast16_t> in) {
	// HUSK: Længde af char er ikke nødvendigvis 8 bit.
}
