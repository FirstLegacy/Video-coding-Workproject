#include "stdafx.h"
#include "DeDCT.h"
#include "DeToBlock.h"

#include <array>

// BinDCT coding.
void DeDCT::invBinDCT(int_fast16_t *arr, uint_fast8_t *out) {
	// Note that the proposed pipeline in the article had an error, so the matrix from it has been used instead.
	// Also note the commented equations are how it's written, but they are unnecessary, though kept in for good measure.

	int_fast16_t d0, c7, d3, d6, d1, d5, d2, d4;

	std::array<int_fast16_t, mBlockSize> buffer;

	// For both dimensions (horizonal / vertical)
	for (size_t j = 0; j < 2; ++j) {
		// For every row or column in the block.
		for (size_t i = 0; i < blockSize; ++i) {
			if (j == 0) {
				d0 = arr[i + 0 * blockSize] * 2; // e0
				c7 = arr[i + 1 * blockSize] * 2; // e1
				d3 = arr[i + 2 * blockSize] * 2; // e2
				d6 = arr[i + 3 * blockSize] * 2; // e3
				d1 = arr[i + 4 * blockSize] * 2; // e4
				d5 = arr[i + 5 * blockSize] * 2; // e5
				d2 = arr[i + 6 * blockSize] * 2; // e6
				d4 = arr[i + 7 * blockSize] * 2; // e7
			}
			else {
				d0 = buffer[0 + i * blockSize];
				c7 = buffer[1 + i * blockSize];
				d3 = buffer[2 + i * blockSize];
				d6 = buffer[3 + i * blockSize];
				d1 = buffer[4 + i * blockSize];
				d5 = buffer[5 + i * blockSize];
				d2 = buffer[6 + i * blockSize];
				d4 = buffer[7 + i * blockSize];
			}

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

			// Stage 4:
			int_fast16_t
				c0 = d0 / 2 + d1,
				c1 = d0 / 2 - d1,
				c2 = d3 * 3 / 8 + d2 * 55 / 64,
				c3 = d3 - d2 * 3 / 8,
				c4 = d4 + c7 / 8,
				c5 = d5 * 9 / 16 - d6 * 7 / 8,
				c6 = d6 + d5 / 2;
				// c7 = d7;

			// Stage 3:
			int_fast16_t
				b0 = (c0 + c3) / 2,
				b1 = (c1 + c2) / 2,
				b2 = (c1 - c2) / 2,
				b3 = (c0 - c3) / 2,
				b4 = (c4 + c5) / 2,
				b5 = (c4 - c5) / 2,
				b6 = (c7 - c6) / 2,
				b7 = (c6 + c7) / 2;

			// Stage 2:
			int_fast16_t
				// a0 = b0,
				// a1 = b1,
				// a2 = b2,
				// a3 = b3,
				// a4 = b4,
				// a7 = b7,
				a5 = b6 * 5 / 8 - b5,
				a6 = b5 * 3 / 8 + b6 * 49 / 64;

			// Stage 1:
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
				buffer[i + 0 * blockSize] = x0;
				buffer[i + 1 * blockSize] = x1;
				buffer[i + 2 * blockSize] = x2;
				buffer[i + 3 * blockSize] = x3;
				buffer[i + 4 * blockSize] = x4;
				buffer[i + 5 * blockSize] = x5;
				buffer[i + 6 * blockSize] = x6;
				buffer[i + 7 * blockSize] = x7;
			}
			else {
				out[0 + i * blockSize] = x0;
				out[1 + i * blockSize] = x1;
				out[2 + i * blockSize] = x2;
				out[3 + i * blockSize] = x3;
				out[4 + i * blockSize] = x4;
				out[5 + i * blockSize] = x5;
				out[6 + i * blockSize] = x6;
				out[7 + i * blockSize] = x7;
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

void normalInverseDCT(int_fast16_t *arr, uint_fast8_t *out) {
	std::array<double, mBlockSize> buffer;
	const static double scale = 2 / sqrt(mBlockSize);
	const static double m_sqrt2 = 1 / sqrt(2);
	double c_m, c_n;

	for (size_t row = 0; row < blockSize; ++row) { // For every row
		for (size_t k = 0; k < blockSize; ++k) { // For every element
			c_m = row == 0 ? m_sqrt2 : 1;
			c_n = k == 0 ? m_sqrt2 : 1;
			double result = 0;
			for (size_t n = 0; n < blockSize; ++n) { // For every element (again)
				result += c_m * c_n * arr[row * blockSize + n] * cos((3.14159265359 / blockSize) * (k + 0.5) * n);
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
				result += c_m * c_n * buffer.at(col + blockSize * n) * cos((3.14159265359 / blockSize) * (k + 0.5) * n);
			}
			out[col + blockSize * k] = (int_fast16_t)(scale * result);
		}
	}
}

void normalInverse2DCT(int_fast16_t *arr, uint_fast8_t *out) {
	double c_m;
	double c_n;
	const static double scale = 2/sqrt(mBlockSize);
	const static double m_sqrt2 = 1/sqrt(2);
	const static double pi = 3.14159265359;


	for (size_t x = 0; x < blockSize; ++x) { // For every row
		for (size_t y = 0; y < blockSize; ++y) { // For every element
			double result = 0.0;
			for (size_t m = 0; m < blockSize; ++m) {
				for (size_t n = 0; n < blockSize; ++n) {
					c_m = m == 0 ? m_sqrt2 : 1;
					c_n = n == 0 ? m_sqrt2 : 1;
					result += c_m * c_n * arr[m * blockSize + n]
						* cos(((2 * x + 1) * m * pi) / (2 * blockSize))
						* cos(((2 * y + 1) * n * pi) / (2 * blockSize));
				}
			}
			out[x * blockSize + y] = (uint_fast8_t)(scale * result);
		}
	}
}
*/


std::vector<unsigned char> DeDCT::deDCT(std::vector<int_fast16_t> in)
{
	std::vector<uint_fast8_t> out(img_res_ycbcr);
	
	// Runs for every block in the image, first the rows, then the columns.

	for (size_t i = 0; i < img_block_count; ++i) {
		invBinDCT(&in[i * mBlockSize], &out[i * mBlockSize]);
	}

	return deToBlock::deBlockify(out);
}
