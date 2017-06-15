#pragma once

class DeDCT
{
public:
	static std::vector<unsigned char> DeDCT::deDCT(std::vector<double>);
private:
	static void invBinDCT(int_fast16_t*, uint_fast8_t*);
};

