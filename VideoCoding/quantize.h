#pragma once

#include <array>

class Quantize
{
public:
	static std::vector<char> quant(std::vector<int_fast16_t>);
	static void setQuality(uint_fast8_t);
private:
	const static std::array<int_fast16_t, 64> lumTableOrig;
	const static std::array<int_fast16_t, 64> chromTableOrig;
	static std::array<int_fast16_t, 64> lumTable;
	static std::array<int_fast16_t, 64> chromTable;
};

