#pragma once

class DCT
{
public:
	static std::vector<char> transform(std::vector<uint_fast8_t> in);
private:
	static void binDCT(uint_fast8_t*, int_fast16_t*);
};

