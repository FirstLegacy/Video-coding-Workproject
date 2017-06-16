#pragma once

#include <array>

class ZigZag
{
public:
	static std::vector<unsigned char> zigzag(std::vector<int_fast16_t>);
private:
	const static std::array<uint_fast8_t, mBlockSize> zigzagOrder;
};

