#pragma once

#include <vector>
#include <array>

class ToBlock
{
public:
	static std::vector<char> blockify(std::array<uint_fast8_t, img_res_ycbcr>);
};

