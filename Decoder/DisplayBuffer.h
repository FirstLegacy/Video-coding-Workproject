#pragma once

#include <vector>
#include <set>

class DisplayBuffer
{
public:
	static void add(std::vector<char>);
private:
	struct img_frame;
	static std::set<img_frame> img_queue;
};

