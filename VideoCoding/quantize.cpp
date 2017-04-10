#include "stdafx.h"
#include "Quantize.h"

#include <vector>

static uint_fast8_t quality = 100; // 100 is max quality, 1 is min quality
const static std::array<uint_fast8_t, 64> lumTable; // Luminance Quantization Table
const static std::array<uint_fast8_t, 64> chromTable; // Chrominance Quantization Table

std::vector<uint8_t> Quantize::quant(std::vector<uint_fast8_t>) {

}