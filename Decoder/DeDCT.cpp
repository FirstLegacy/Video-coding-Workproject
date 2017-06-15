#include "stdafx.h"
#include "DeDCT.h"
#include "DeToBlock.h"

#include <array>

// BinDCT coding.
void DeDCT::invBinDCT(int_fast16_t *arr, uint_fast8_t *out) {
	// Note that the proposed pipeline in the article had an error, so the matrix from it has been used instead.
	// Also note the commented equations are how it's written, but they are unnecessary, though kept in for good measure.

	int_fast16_t d0, c7, d3, d6, d1, d5, d2, d4;

	// For both dimensions (horizonal / vertical)
	for (size_t j = 0; j < 2; ++j) {
		// For every row or column in the block.
		for (size_t i = 0; i < blockSize; ++i) {
			if (j == 0) {
				d0 = arr[0 + i * blockSize];
				c7 = arr[1 + i * blockSize];
				d3 = arr[2 + i * blockSize];
				d6 = arr[3 + i * blockSize];
				d1 = arr[4 + i * blockSize];
				d5 = arr[5 + i * blockSize];
				d2 = arr[6 + i * blockSize];
				d4 = arr[7 + i * blockSize];
			}
			else {
				d0 = out[i + 0 * blockSize];
				c7 = out[i + 1 * blockSize];
				d3 = out[i + 2 * blockSize];
				d6 = out[i + 3 * blockSize];
				d1 = out[i + 4 * blockSize];
				d5 = out[i + 5 * blockSize];
				d2 = out[i + 6 * blockSize];
				d4 = out[i + 7 * blockSize];
			}

			// Stage 1:
			/*
			uint_fast16_t
			e0 = d0,
			e1 = c7, // e1 = d7,
			e2 = d3,
			e3 = d6,
			e4 = d1,
			e5 = d5,
			e6 = d2,
			e7 = d4;
			*/

			// Stage 2:
			int_fast16_t
				c0 = d0 + d1,
				c1 = (d0 - d1) / 2,
				c2 = d2 - d3 * 3 / 8,
				c3 = d2 * 3 / 8 + d3 * 55 / 64,
				c4 = d4 - c7 / 8,
				c5 = d5 + d6 * 7 / 8,
				c6 = d6 * 9 / 16 - d5 / 2;
				// d7 = c7;
				/*
				d0 = c0 + c1,
				d1 = (c0 - c1) / 2,
				d2 = c2 - (c3 + (c3 << 1)) >> 3,
				d3 = (d2 + (d2 << 1)) >> 3 - c3,
				d4 = c4 - c7 / 8,
				d5 = c5 + ((c6 << 3) - c6) >> 3,
				d6 = c6 - (d5 >> 1);
				*/

			// Stage 3:
			int_fast16_t
				b0 = c0 + c3, // c0 = b0 + b3,
				b1 = c1 + c2, // c1 = b1 + b2,
				b2 = c1 - c2, // c2 = b1 - b2,
				b3 = c0 - c3, // c3 = b0 - b3,
				b4 = c4 + c5, // c4 = b4 + b5,
				b5 = c4 - c5, // c5 = b4 - b5,
				b6 = c7 - c6, // c6 = b7 - b6,
				b7 = c6 + c7; // c7 = b6 + b7;

			// Stage 4:
			int_fast16_t
				// b0 = a0,
				// b1 = a1,
				// b2 = a2,
				// b3 = a3,
				// b4 = a4,
				// b7 = a7,
				a5 = b6 * 5 / 8 - b5 * 49 / 64,
				a6 = b5 * 3 / 8 + b6;
			/*
			b6 = a6 + (a5 + (a5 << 1)) >> 3,
			b5 = (b6 + (b6 << 2)) >> 3 - a5;
			*/

			// Stage 5:
			int_fast16_t
				x0 = b0 + b7,
				x1 = b1 + a6,
				x2 = b2 + a5,
				x3 = b3 + b4,
				x4 = b3 - b4,
				x5 = b2 - a5,
				x6 = b1 - a6,
				x7 = b0 - b7;

			if (j == 0) {
				out[0 + i * blockSize] = x0;
				out[1 + i * blockSize] = x1;
				out[2 + i * blockSize] = x2;
				out[3 + i * blockSize] = x3;
				out[4 + i * blockSize] = x4;
				out[5 + i * blockSize] = x5;
				out[6 + i * blockSize] = x6;
				out[7 + i * blockSize] = x7;
			}
			else {
				out[i + 0 * blockSize] = x0;
				out[i + 1 * blockSize] = x1;
				out[i + 2 * blockSize] = x2;
				out[i + 3 * blockSize] = x3;
				out[i + 4 * blockSize] = x4;
				out[i + 5 * blockSize] = x5;
				out[i + 6 * blockSize] = x6;
				out[i + 7 * blockSize] = x7;
			}
		}
	}
}

/*
// DCT-III * 2/N -- For testing
void normalInverseDCT(double *arr, uint_fast8_t *out) {
	std::array<double, mBlockSize> buffer;

	for (size_t row = 0; row < blockSize; ++row) { // For every row
		for (size_t k = 0; k < blockSize; ++k) { // For every element
			double result = arr[row * blockSize] * 0.5; // x0
			for (size_t n = 1; n < blockSize; ++n) { // For every element (again)
				result += arr[row * blockSize + n] * cos((3.14159265359 / blockSize) * n * (k + 0.5));
			}
			buffer.at(row * blockSize + k) = result * 2 / blockSize;
		}
	}

	for (size_t col = 0; col < blockSize; ++col) { // For every column
		for (size_t k = 0; k < blockSize; ++k) { // For every element
			double result = buffer[col * blockSize] * 0.5; // x0
			for (size_t n = 1; n < blockSize; ++n) { // For every element (again)
				result += buffer.at(col + blockSize * n) * cos((3.14159265359 / blockSize) * n * (k + 0.5));
			}
			out[col + blockSize * k] = (uint_fast8_t)result * 2 / blockSize;
		}
	}
}
*/

void normalInverseDCT(double *arr, uint_fast8_t *out) {
	std::array<double, mBlockSize> buffer;
	double alpha_u;
	double alpha_v;
	const static double sqrt2 = sqrt(2);
	const static double pi = 3.14159265359;


	for (size_t x = 0; x < blockSize; ++x) { // For every row
		for (size_t y = 0; y < blockSize; ++y) { // For every element
			double result = 0.0;
			for (size_t u = 0; u < blockSize; ++u) {
				for (size_t v = 0; v < blockSize; ++v) {
					alpha_u = u == 0 ? sqrt2 : 1;
					alpha_v = v == 0 ? sqrt2 : 1;
					result += alpha_u * alpha_v * arr[v * blockSize + u]
						* cos(((2 * x + 1) * u * pi) / 16)
						* cos(((2 * y + 1) * v * pi) / 16);
				}
			}
			out[y * blockSize + x] = result / 4;
		}
	}
}


std::vector<unsigned char> DeDCT::deDCT(std::vector<double> in)
{
	std::vector<uint_fast8_t> out(img_res_ycbcr);
	
	// Runs for every block in the image, first the rows, then the columns.

	for (size_t i = 0; i < img_block_count; ++i) {
		//invBinDCT(&in[i * mBlockSize], &out[i * mBlockSize]);
		normalInverseDCT(&in[i * mBlockSize], &out[i * mBlockSize]);
	}

	return deToBlock::deBlockify(out);
}
