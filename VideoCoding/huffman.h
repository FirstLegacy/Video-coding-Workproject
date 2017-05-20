#pragma once

#include <functional>
#include <vector>
#include <array>
#include <limits>
#include <bitset>

using bitvec = std::vector<bool>; // Alias for std::vector<bool>. std::vector<bool> is actually a bit vector, not boolean.
								  // This alias is used for readability and clarity.

class Huffman
{
private:
	// const static std::map<uint_fast8_t, bitvec> acLumTable;
	// const static std::map<uint_fast8_t, bitvec> acChromTable;
	const static std::array<bitvec, 12> dcChromTable;
	const static std::array<bitvec, 12> dcLumTable;
	const static std::array<bitvec, 11> acTable;
	const static std::vector<std::bitset<11>> signValueTable;
	const static std::array<int_fast16_t, 10> two_pow;
	static void insertLength(bitvec&, size_t, int_fast8_t);
	static void insertValue(bitvec&, size_t, int_fast16_t);
	static void inserter(bitvec&, int_fast16_t, int_fast8_t);
public:
	static std::vector<char> huff(std::vector<int_fast16_t>);
};

