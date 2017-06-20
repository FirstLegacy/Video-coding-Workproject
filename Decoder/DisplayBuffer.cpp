#include "stdafx.h"
#include "DisplayBuffer.h"
#include "DeHuffman.h"

#include <chrono>
#include <thread>

struct DisplayBuffer::ImgFrame {
	uint32_t count;
	mutable std::vector<std::vector<char>> parts;
	mutable uint8_t part_amount;
	mutable uint8_t parts_contained;
	mutable size_t creationTime;
	
	ImgFrame(uint32_t c, uint8_t p_amount,
		std::vector<char> part, uint8_t part_count) {
		count = c;
		part_amount = p_amount;
		parts.resize(p_amount);
		parts.at(part_count) = part;

		parts_contained = 1;
		creationTime = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		).count();
	}

	void addPart(std::vector<char> part, uint8_t part_count) const {
		parts.at(part_count) = part;
		++parts_contained;
	}
};

struct DisplayBuffer::MergedImg {
	uint32_t count;
	std::vector<unsigned char> image;
	bool ditched_prev;

	bool operator<(const MergedImg &mi) const
	{
		return (mi.count < count);
	}
	bool operator==(const MergedImg &mi) const // This is probably not needed, but having it doesn't hurt.
	{
		return (mi.count == count);
	}

	MergedImg(uint32_t frameNumber, std::vector<unsigned char> img, bool ditched) {
		count = frameNumber;
		image = img;
		ditched_prev = ditched;
	}
};

std::atomic_size_t DisplayBuffer::lastShownFrame = 0;
int DisplayBuffer::bufferDeleteTime;
std::priority_queue<DisplayBuffer::MergedImg> DisplayBuffer::img_queue;
std::vector<DisplayBuffer::ImgFrame> DisplayBuffer::unfinished_frames;
std::shared_mutex DisplayBuffer::imgq_mut, DisplayBuffer::uimg_mut;
std::atomic_uint DisplayBuffer::runningThreads = 0;

void DisplayBuffer::insertImage(ImgFrame i_f, bool ditched_prev) {
	std::vector<char> merged;

	for (const auto &p : i_f.parts) {
		merged.insert(merged.end(), p.begin(), p.end());
	}

	auto image = DeHuffman::huff(merged); // Generate the image.

	std::lock_guard<std::shared_mutex> ulock(imgq_mut); // Write lock
	img_queue.emplace(i_f.count, image, ditched_prev);

	--runningThreads;
}

void DisplayBuffer::mergeHandler()
{
	auto suppThreads = std::thread::hardware_concurrency();
	bool ditched_prev = false; // If previous image was ditched for whatever reason
							   // tell it to the getter so it doesn't wait for earlier images.

	while (true) {
		if (runningThreads < suppThreads) {
			size_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()
			).count(); // Current time in milliseconds since january 1st 1970
			
			std::lock_guard<std::shared_mutex> lockg(uimg_mut);
			for (size_t i = 0; i < unfinished_frames.size(); ++i) {
				auto entry = unfinished_frames.at(i);
				if (entry.count < lastShownFrame) {
					goto delete_entry; // goto is underrated.
				}
				else if (entry.parts_contained == entry.part_amount) {
					++runningThreads;

					auto i_f = entry;
					unfinished_frames.erase(unfinished_frames.begin() + i);	// It has to be erased.
					--i;

					std::thread ii(insertImage, i_f, ditched_prev);
					ii.detach();
					ditched_prev = false;
					break;
				}
				else if (now - bufferDeleteTime > entry.creationTime) {
				delete_entry:
					ditched_prev = true;
					// If it has been in the buffer for too long or is outdated.
					unfinished_frames.erase(unfinished_frames.begin() + i);	// It has to be erased.
					--i;
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Sleep a bit to avoid spamming requests.
	}
}

void DisplayBuffer::add(std::vector<char> newPart) {
	uint32_t frame_count = (newPart.at(1) << 24 & 0xFF000000 | newPart.at(2) << 16 & 0x00FF0000
		| newPart.at(3) << 8 & 0x0000FF00 | newPart.at(4) & 0x000000FF);
	uint8_t part_count = newPart.at(5);
	uint8_t part_amount = newPart.at(6);
	std::vector<char> part(newPart.begin() + 7, newPart.end());

	if (frame_count >= lastShownFrame)
	{
		std::lock_guard<std::shared_mutex> ulock(uimg_mut); // Write lock
		for (const auto &entry : unfinished_frames) {
			if (entry.count == frame_count) {
				entry.addPart(part, part_count);
				return;
			}
		}
		unfinished_frames.push_back(ImgFrame(frame_count, part_amount, part, part_count));
	}
}

bool DisplayBuffer::get(std::vector<unsigned char> &image) {
	std::lock_guard<std::shared_mutex> lock(imgq_mut); // Write lock, only this can write.
	const static size_t max_wait_time = 50; // Don't wait more than 50 ms.
	size_t ms_waited = 0;

	while (!img_queue.empty()) { // If there is any frames in the buffer.
		auto first_entry = img_queue.top(); // Get the first one (with the lowest frame number).

		if (first_entry.count < lastShownFrame) {
			img_queue.pop();
		}
		else if (first_entry.count > lastShownFrame + 1 && !first_entry.ditched_prev && ms_waited < max_wait_time) {
			ms_waited += 1;
			std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Wait a little if previous frame hasn't arrived yet.
		} else {
			lastShownFrame = first_entry.count;
			image = first_entry.image; // Get the image from that.
			img_queue.pop();

			return true;
		} 
	}
	return false;
}
