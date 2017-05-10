#pragma once

#include <vector>
#include <array>

class DCT
{
public:
	static std::vector<char> transform(std::array<uint_fast8_t, img_res_ycbcr> in);
private:
	static void binDCT(uint_fast8_t*, uint_fast16_t*, bool);
};

