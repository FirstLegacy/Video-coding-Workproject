#pragma once

#include <set>

class DisplayBuffer
{
public:
	static void add(std::vector<char>);
private:
	struct ImgFrame;
	static std::set<ImgFrame> img_queue;
};

