#pragma once

#include <vector>
#include <array>

class Runlength
{
public:
	static std::vector<char> run(std::array<uint_fast16_t, img_res_ycbcr>);
};

