#pragma once

#include <queue>
#include <shared_mutex>
#include <atomic>

class DisplayBuffer
{
private:
	struct ImgFrame;
	struct MergedImg;
	static std::vector<ImgFrame> unfinished_frames;
	static std::priority_queue<MergedImg> img_queue;
	static std::atomic_size_t lastShownFrame;
	static std::shared_mutex imgq_mut, uimg_mut;
	static void insertImage(ImgFrame, bool);
	static std::atomic_uint runningThreads;
public:
	static void add(std::vector<char>);
	static bool get(std::vector<unsigned char>&);
	static void mergeHandler();
	static int bufferDeleteTime;
};
