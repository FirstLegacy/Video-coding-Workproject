#pragma once
class DCT
{
public:
	static std::vector<char> transform(std::array<uint_fast8_t, img_res_ycbcr>);
private:
	static void binDCT(std::array<uint_fast8_t, mBlockSize> &, bool);
};

