#pragma once
class Huffman
{
private:
	const static std::map<uint_fast8_t, bitvec> dcLumTable;
	const static std::map<uint_fast8_t, bitvec> acLumTable;
	const static std::map<uint_fast8_t, bitvec> dcChromTable;
	const static std::map<uint_fast8_t, bitvec> acChromTable;
public:
	static std::vector<uint8_t> huff(std::vector<uint_fast8_t>);
};

