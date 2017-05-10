#pragma once
ref class deYCbCrToRgb sealed
{
public:
	void upSampling(std::array<uint8_t, (img_res * 3) / 4>& in);
	deYCbCrToRgb();
};

