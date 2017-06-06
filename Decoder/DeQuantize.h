#pragma once

#include <vector>

class DeQuantize
{
public:
	static std::vector<unsigned char> dequant(std::vector<int_fast16_t>);
private:
	static void DeQuantize::setQuality(uint_fast8_t);
};

