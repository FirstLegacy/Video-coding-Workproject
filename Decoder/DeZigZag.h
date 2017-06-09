#pragma once

#include <array>

class DeZigZag
{
public:
	static std::vector<unsigned char> unzigzag(std::vector<int_fast16_t>);
private:
	const static std::array<uint_fast8_t, mBlockSize> zigzagOrder;
};
