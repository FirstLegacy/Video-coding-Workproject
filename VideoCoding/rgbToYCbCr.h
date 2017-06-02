#pragma once

#include <vector>
#include <array>

class RgbToYCbCr
{
public:
	static std::vector<char> convert(std::array<uint8_t, img_res_rgb>);
	static void downSampling(std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>*);
};

