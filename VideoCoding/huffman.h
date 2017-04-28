#pragma once
class Huffman
{
private:
	const static std::map<uint_fast8_t, bitvec> dcLumTable;
	const static std::map<uint_fast8_t, bitvec> acLumTable;
	const static std::map<uint_fast8_t, bitvec> dcChromTable;
	const static std::map<uint_fast8_t, bitvec> acChromTable;
	const static std::map<short, bitvec> signValueTable;
public:
	static std::vector<char> huff(std::vector<uint_fast16_t>);
};

