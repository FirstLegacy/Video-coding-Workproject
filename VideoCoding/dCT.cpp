#include "stdafx.h"
#include "DCT.h"
#include "Quantize.h"

#include <array>

#include "Decoder\DeDCT.h"

// Implements BinDCT, source: https://pdfs.semanticscholar.org/e024/bdc2b5b6db2d0eed65ca96ae575b600fa3a9.pdf

// BinDCT coding.
void DCT::binDCT(uint_fast8_t *arr, int_fast16_t *out) {
	// Note that the proposed pipeline in the article had an error, so the matrix from it has been used instead.
	// Also note the commented equations are how it's written, but they are unnecessary, though kept in for good measure.

	int_fast16_t x0, x1, x2, x3, x4, x5, x6, x7;

	// For both dimensions (horizonal / vertical)
	for (size_t j = 0; j < 2; ++j) {
		// For every row or column in the block.
		for (size_t i = 0; i < blockSize; ++i) {
			if (j == 0) {
				x0 = arr[0 + i * blockSize];
				x1 = arr[1 + i * blockSize];
				x2 = arr[2 + i * blockSize];
				x3 = arr[3 + i * blockSize];
				x4 = arr[4 + i * blockSize];
				x5 = arr[5 + i * blockSize];
				x6 = arr[6 + i * blockSize];
				x7 = arr[7 + i * blockSize];
			}
			else {
				x0 = out[i + 0 * blockSize];
				x1 = out[i + 1 * blockSize];
				x2 = out[i + 2 * blockSize];
				x3 = out[i + 3 * blockSize];
				x4 = out[i + 4 * blockSize];
				x5 = out[i + 5 * blockSize];
				x6 = out[i + 6 * blockSize];
				x7 = out[i + 7 * blockSize];
			}

			// Stage 1:
			int_fast16_t
				a0 = x0 + x7,
				a1 = x1 + x6,
				a2 = x2 + x5,
				a3 = x3 + x4,
				a4 = x3 - x4,
				a5 = x2 - x5,
				a6 = x1 - x6,
				a7 = x0 - x7;

			// Stage 2:
			int_fast16_t
				// b0 = a0,
				// b1 = a1,
				// b2 = a2,
				// b3 = a3,
				// b4 = a4,
				// b7 = a7,
				b5 = a6 * 5 / 8 - a5 * 49 / 64,
				b6 = a5 * 3 / 8 + a6;
				/*
				b6 = a6 + (a5 + (a5 << 1)) >> 3,
				b5 = (b6 + (b6 << 2)) >> 3 - a5;
				*/

			// Stage 3:
			int_fast16_t
				c0 = a0 + a3, // c0 = b0 + b3,
				c1 = a1 + a2, // c1 = b1 + b2,
				c2 = a1 - a2, // c2 = b1 - b2,
				c3 = a0 - a3, // c3 = b0 - b3,
				c4 = a4 + b5, // c4 = b4 + b5,
				c5 = a4 - b5, // c5 = b4 - b5,
				c6 = a7 - b6, // c6 = b7 - b6,
				c7 = b6 + a7; // c7 = b6 + b7;

			// Stage 4:
			int_fast16_t
				d0 = c0 + c1,
				d1 = (c0 - c1) / 2,
				d2 = c2 - c3 * 3 / 8,
				d3 = c2 * 3 / 8 + c3 * 55 / 64,
				d4 = c4 - c7 / 8,
				d5 = c5 + c6 * 7 / 8,
				d6 = c6 * 9 / 16 - c5 / 2;
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

			// Stage 5:
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

			if (j == 0) {
				out[0 + i * blockSize] = d0 / 2;
				out[1 + i * blockSize] = c7 / 2;
				out[2 + i * blockSize] = d3 / 2;
				out[3 + i * blockSize] = d6 / 2;
				out[4 + i * blockSize] = d1 / 2;
				out[5 + i * blockSize] = d5 / 2;
				out[6 + i * blockSize] = d2 / 2;
				out[7 + i * blockSize] = d4 / 2;
			}
			else {
				out[i + 0 * blockSize] = d0 / 2;
				out[i + 1 * blockSize] = c7 / 2;
				out[i + 2 * blockSize] = d3 / 2;
				out[i + 3 * blockSize] = d6 / 2;
				out[i + 4 * blockSize] = d1 / 2;
				out[i + 5 * blockSize] = d5 / 2;
				out[i + 6 * blockSize] = d2 / 2;
				out[i + 7 * blockSize] = d4 / 2;
			}
		}
	}
}


// NormalDCT-II (FOR TEST)
void normalDCT(uint_fast8_t *arr, int_fast16_t *out) {
	std::array<double, mBlockSize> buffer;
	const static double scale = 2 / sqrt(mBlockSize);
	const static double m_sqrt2 = 1 / sqrt(2);
	double c_m, c_n;
	const static double pi = 3.14159265359;

	for (size_t row = 0; row < blockSize; ++row) { // For every row
		for (size_t k = 0; k < blockSize; ++k) { // For every element
			double result = 0;
			for (size_t n = 0; n < blockSize; ++n) { // For every element (again)
				result += arr[row * blockSize + n] * cos((pi / blockSize) * (n + 0.5) * k);
			}
			buffer.at(row * blockSize + k) = result;
		}
	}

	for (size_t col = 0; col < blockSize; ++col) { // For every column
		for (size_t k = 0; k < blockSize; ++k) { // For every element
			c_m = col == 0 ? m_sqrt2 : 1;
			c_n = k == 0 ? m_sqrt2 : 1;
			double result = 0;
			for (size_t n = 0; n < blockSize; ++n) { // For every element (again)
				result += buffer.at(col + blockSize * n) * cos((pi / blockSize) * (n + 0.5) * k);
			}
			out[col + blockSize * k] = (int_fast16_t)(scale * c_m * c_n * result);
		}
	}
}

void normalDCT2(uint_fast8_t *arr, int_fast16_t *out) {
	double c_m;
	double c_n;
	const static double scale = 2 / sqrt(mBlockSize);
	const static double m_sqrt2 = 1 / sqrt(2);
	const static double pi = 3.14159265359;


	for (size_t m = 0; m < blockSize; ++m) { // For every row
		for (size_t n = 0; n < blockSize; ++n) { // For every element
			double result = 0.0;
			c_m = m == 0 ? m_sqrt2 : 1;
			c_n = n == 0 ? m_sqrt2 : 1;
			for (size_t x = 0; x < blockSize; ++x) {
				for (size_t y = 0; y < blockSize; ++y) {
					result += arr[m * blockSize + n]
						* cos(((2 * x + 1) * m * pi) / (2 * blockSize))
						* cos(((2 * y + 1) * n * pi) / (2 * blockSize));
				}
			}
			out[m * blockSize + n] = scale * c_m * c_n * result;
		}
	}
}


// Performs DCT transformation.
std::vector<unsigned char> DCT::transform(std::vector<uint_fast8_t> in) {	
	std::vector<int_fast16_t> out(img_res_ycbcr);
	std::vector<int_fast16_t> zout(img_res_ycbcr);

	// Runs for every block in the image, first the rows, then the columns.
	for (size_t i = 0; i < img_block_count; ++i) {
		//binDCT(&in[i * mBlockSize], &out[i * mBlockSize]);
		normalDCT(&in[i * mBlockSize], &out[i * mBlockSize]);
	}
	
	// return Quantize::quant(out);
	auto a = zout;
	return DeDCT::deDCT(out);
}
