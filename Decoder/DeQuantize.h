#pragma once
ref class DeQuantize sealed
{
public:
	DeQuantize();
	std::vector<char> dequant(std::array<uint_fast16_t, img_res_ycbcr> in);
};

