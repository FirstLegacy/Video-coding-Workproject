#pragma once

class deYCbCrToRgb
{
public:
	static std::vector<unsigned char> upSampling(std::vector<uint_fast8_t>);
private:
	static std::vector<unsigned char> yCbCrToRGB(std::vector<uint_fast8_t>,
							std::vector<uint_fast8_t>, std::vector<uint_fast8_t>);
};

