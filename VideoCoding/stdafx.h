// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <vector> // Everything uses vector so it's included here.

extern const size_t blockSize = 8;
extern const size_t mBlockSize = blockSize * blockSize;
extern const size_t img_res_w = 640;
extern const size_t img_res_h = 480;
extern const size_t img_res = img_res_w * img_res_h;
extern const size_t img_res_rgb = img_res * 3;
extern const size_t img_res_cbcr = (img_res / 4) * 2;
extern const size_t img_res_ycbcr = img_res + img_res_cbcr;
extern const size_t img_block_count = img_res_ycbcr / mBlockSize;
extern const size_t img_y_dc_values = img_res / mBlockSize; // The amount of DC values in the luminance part.
