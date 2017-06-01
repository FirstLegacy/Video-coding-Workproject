#pragma once

#include <vector>
#include <array>

class ZigZag
{
public:
	static std::vector<char> zigzag(std::array<uint_fast16_t, img_res_ycbcr>);
private:
	const static std::array<uint_fast8_t, mBlockSize> zigzagOrder;
};

