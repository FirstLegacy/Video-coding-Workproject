#pragma once

#include <vector>
#include <array>

class ZigZag
{
public:
	static std::vector<char> zigzag(std::vector<int_fast16_t>);
private:
	const static std::array<uint_fast8_t, mBlockSize> zigzagOrder;
};

