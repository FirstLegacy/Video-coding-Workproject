#include "stdafx.h"
#include "Huffman.h"

#include <vector>
#include <map>

using bitvec = std::vector<bool>;

const std::map<uint_fast8_t, bitvec> dcLumTable;
const std::map<uint_fast8_t, bitvec> acLumTable;
const std::map<uint_fast8_t, bitvec> dcChromTable;
const std::map<uint_fast8_t, bitvec> acChromTable;

// Returns byte-array
std::vector<uint8_t> Huffman::huff(std::vector<uint_fast8_t>) {

}
