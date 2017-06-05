#pragma once

#include <vector>
#include <array>

class RgbToYCbCr
{
public:
	static std::vector<char> convert(std::vector<unsigned char>);
private:
	static void downSampling(std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>*);
};

