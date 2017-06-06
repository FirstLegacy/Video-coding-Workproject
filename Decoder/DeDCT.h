#pragma once

#include <vector>

class DeDCT
{
public:
	static std::vector<unsigned char> DeDCT::deDCT(std::vector<int_fast16_t>);
private:
	static void invBinDCT(int_fast16_t*, uint_fast8_t*);
};

