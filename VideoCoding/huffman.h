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
	const static std::array<bitvec, 12> dcChromTable;
	const static std::array<bitvec, 12> dcLumTable;
	const static std::array<bitvec, 11> acTable;
	const static std::array<bitvec, 6> zeroLengthTable;
	const static std::vector<std::bitset<11>> signValueTable;
	const static std::vector<std::bitset<5>> zeroValueTable;
	const static std::array<int_fast16_t, 10> two_pow;
	static void insertBits(std::vector<char>&, bitvec, uint_fast8_t&);
	template <size_t size>
	static void insertBits(std::vector<char>&, std::bitset<size>, size_t, uint_fast8_t&);
	static void insertLength(std::vector<char>&, size_t, int_fast8_t, uint_fast8_t&);
	static void insertSignedValue(std::vector<char>&, size_t, int_fast16_t, uint_fast8_t&);
	static void insertZeroValue(std::vector<char>&, size_t, int_fast16_t, uint_fast8_t&);
	static void inserter(std::vector<char>&, int_fast16_t, int_fast16_t, int_fast8_t, uint_fast8_t&);
public:
	static std::vector<char> huff(std::vector<int_fast16_t>);
};

