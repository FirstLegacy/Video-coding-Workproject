#pragma once

#include <set>

class DisplayBuffer
{
public:
	static void add(std::vector<char>);
	static bool get(std::vector<unsigned char>&);
private:
	struct ImgFrame;
	static std::set<ImgFrame> img_queue;
};

