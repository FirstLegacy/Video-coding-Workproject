#pragma once

class Quantize
{
public:
	static std::vector<char> quant(std::array<uint_fast16_t, img_res_ycbcr>);
private:
	static uint_fast8_t quality;
	const static std::array<uint_fast16_t, 64> lumTable;
	const static std::array<uint_fast16_t, 64> chromTable;
};

