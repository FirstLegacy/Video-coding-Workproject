#include "stdafx.h"
#include "DisplayBuffer.h"
#include "DeHuffman.h"

struct DisplayBuffer::ImgFrame {
	uint32_t count;
	uint8_t part_amount;
	uint8_t parts_contained;
	std::vector<std::vector<char>> parts;
	std::vector<unsigned char> image;
	bool assembled = false;

	bool operator<(const ImgFrame &i_f) const
	{
		return count < i_f.count;
	}
	bool operator==(const ImgFrame& i_f) const
	{
		return count == i_f.count;
	}

	ImgFrame(uint32_t c, uint8_t p_amount,
		std::vector<char> part, uint8_t part_count) {
		count = c;
		part_amount = p_amount;
		parts.resize(p_amount);
		parts.at(part_count) = part;

		parts_contained = 1;

		merge();
	}

	void addPart(std::vector<char> part, uint8_t part_count) {
		parts.at(part_count) = part;
		++parts_contained;

		merge();
	}

	void merge() {
		if (parts_contained == part_amount) {
			std::vector<char> merged;
			for (const auto &p : parts) {
				merged.insert(merged.end(), p.begin(), p.end());
			}
			image = DeHuffman::huff(merged);

			assembled = true;
		}
	}
};

std::set<DisplayBuffer::ImgFrame> DisplayBuffer::img_queue;

void DisplayBuffer::add(std::vector<char> newPart) {
	uint32_t frame_count = (newPart.at(4) << 24 | newPart.at(3) << 16 | newPart.at(2) << 8 | newPart.at(1));
	uint8_t part_count = newPart.at(5);
	uint8_t part_amount = newPart.at(6);
	std::vector<char> part(newPart.begin() + 7, newPart.end());

	bool found = false;

	for (auto &entry : img_queue) {
		if (entry.count == frame_count) {
			found = true;

			auto newEntry = entry;
			img_queue.erase(entry);
			newEntry.addPart(part, part_count);

			img_queue.insert(newEntry);
			break;
		}
	}
	if (!found) {
		img_queue.insert(ImgFrame(frame_count, part_amount, part, part_count));
	}
}
