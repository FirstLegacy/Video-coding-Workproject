#pragma once

#include <functional>
#include <vector>
#include <array>
#include <limits>
#include <bitset>
#include <map>

using bitvec = std::vector<bool>; // Alias for std::vector<bool>. std::vector<bool> is actually a bit vector, not boolean.
								  // This alias is used for readability and clarity.

class DeHuffman
{
private:
	const static std::array<bitvec, 12> dcChromTable;
	const static std::array<bitvec, 12> dcLumTable;
	const static std::array<bitvec, 11> acTable;
	const static std::array<bitvec, 10> zeroLengthTable;
	const static std::map<std::bitset<11>, int_fast16_t> signValueMap;
	static size_t getLength(bitvec, int_fast8_t);
	static int_fast16_t getValue(std::vector<int_fast16_t>&, std::vector<char>, size_t, size_t);
public:
	static std::vector<unsigned char> huff(std::vector<char>);
};
