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
	const static std::map<std::bitset<11>, int_fast16_t> signValueTable;
	const static std::array<int_fast16_t, 10> two_pow;
	static size_t getLength(bitvec, int_fast8_t);
	static int_fast16_t getValue(std::vector<int_fast16_t>&, size_t, int_fast16_t);
public:
	static std::vector<int_fast16_t> huff(std::vector<char>);
};
