#include "stdafx.h"
#include "DisplayBuffer.h"
#include "DeHuffman.h"

struct DisplayBuffer::img_frame {
	bool assembled = false;
	uint32_t count;
	uint8_t part_amount;
	uint8_t parts_contained;
	std::vector<std::vector<char>> parts;
	std::vector<unsigned char> image;

	bool operator<(const img_frame &i_f) const
	{
		return count < i_f.count;
	}
	bool operator==(const img_frame& i_f) const
	{
		return count == i_f.count;
	}

	img_frame(uint32_t c, uint8_t p_amount,
		std::vector<char> part, uint8_t part_count) {
		count = c;
		part_amount = p_amount;
		parts.resize(p_amount);
		parts.at(part_count) = part;

		parts_contained = 1;
	}

	void addPart(std::vector<char> part, uint8_t part_count) {
		parts.at(part_count) = part;
		++parts_contained;
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

std::set<DisplayBuffer::img_frame> DisplayBuffer::img_queue;

void DisplayBuffer::add(std::vector<char> newPart) {
	uint32_t frame_count = (newPart.at(1) << 24 | newPart.at(2) << 16 | newPart.at(3) << 8 | newPart.at(4));
	uint8_t part_count = newPart.at(5);
	uint8_t part_amount = newPart.at(6);
	std::vector<char> part(newPart.begin() + 7, newPart.end());

	bool found = false;

	for (auto &entry : img_queue) {
		if (entry.count == frame_count) {
			found = true;

			img_queue.erase(entry);
			auto newEntry = entry;
			newEntry.addPart(part, part_count);

			img_queue.insert(newEntry);
			break;
		}
	}
	if (!found) {
		img_queue.insert(img_frame(frame_count, part_amount, part, part_count));
	}
}
