// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <array>

const static size_t blockSize = 8;
const static size_t mBlockSize = blockSize * blockSize;
const static size_t img_res_w = 640;
const static size_t img_res_h = 480;
const static size_t img_res = img_res_w * img_res_h;
const static size_t img_res_rgb = img_res * 3;
const static size_t img_res_cbcr = (img_res / 4) * 2;
const static size_t img_res_ycbcr = img_res + img_res_cbcr;
const static size_t img_div_blocksize = img_res_ycbcr / mBlockSize;

// Gets 2D position in a flat array.
template<typename T, size_t SIZE>
inline T flatget(const typename std::array<T, SIZE> arr, size_t width, size_t x, size_t y)
{
	return &arr[x + y * width];
};

// TODO: reference additional headers your program requires here
