#pragma once

class Quantize
{
private:
	const static std::vector<uint_fast8_t> lumTable;
	const static std::vector<uint_fast8_t> chromTable;
public:
	static std::vector<uint8_t> quant(std::vector<uint_fast8_t>);
};

