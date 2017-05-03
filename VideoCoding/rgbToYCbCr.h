#pragma once
class RgbToYCbCr
{
public:
	static std::vector<char> convert(std::array<uint8_t, in_res_rgb>);
	void downSampling(std::array<uint8_t, in_res_ycbcr>& in);
};

