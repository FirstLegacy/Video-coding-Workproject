#pragma once

#include <vector>
#include <array>

class Quantize
{
public:
	static std::vector<char> quant(std::array<uint_fast16_t, img_res_ycbcr>);
	static void setQuality(uint_fast16_t);
private:
	// static uint_fast8_t quality;
	const static std::array<uint_fast16_t, 64> lumTableOrig;
	const static std::array<uint_fast16_t, 64> chromTableOrig;
	static std::array<uint_fast16_t, 64> lumTable;
	static std::array<uint_fast16_t, 64> chromTable;
};

