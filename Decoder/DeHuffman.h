#pragma once

#include <array>
#include <bitset>
#include <unordered_map>

using bitvec = std::vector<bool>; // Alias for std::vector<bool>. std::vector<bool> is actually a bit vector, not boolean.
								  // This alias is used for readability and clarity.

namespace std
{
	template<>
	struct hash<bitvec>
	{
		size_t operator()(bitvec const& go_v) const
		{
			size_t seed = go_v.size();
			for (const auto &i : go_v)
			{
				seed ^= hash<bool>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
			return seed;
		}
	};
}

class DeHuffman
{
private:
	const static std::unordered_map<bitvec, int_fast16_t> dcChromMap;
	const static std::unordered_map<bitvec, int_fast16_t> dcLumMap;
	const static std::unordered_map<bitvec, int_fast16_t> acMap;
	const static std::unordered_map<bitvec, int_fast16_t> zeroLengthMap;
	/*
	const static std::array<bitvec, 12> dcChromTable;
	const static std::array<bitvec, 12> dcLumTable;
	const static std::array<bitvec, 11> acTable;
	const static std::array<bitvec, 6> zeroLengthTable;
	*/
	const static std::vector<std::bitset<11>> signValueTable;
	const static std::vector<std::bitset<5>> zeroValueTable;
	const static std::array<int_fast16_t, 12> two_pow;
	static size_t getLength(bitvec, int_fast8_t);
	static int_fast16_t getValue(std::vector<char>, size_t, size_t&, size_t&);
	static int_fast16_t getZeroValue(std::vector<char>, size_t, size_t&, size_t&);
public:
	static std::vector<unsigned char> huff(std::vector<char>);
};
