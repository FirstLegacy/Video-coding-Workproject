#pragma once

class Interface
{
public:
	// static void GUI();
	// static void updateFrame(std::vector<unsigned char>);
	static void init();
	static void update(std::vector<unsigned char>);
private:
	static std::vector<unsigned char> frame;
	static bool updated;
};

