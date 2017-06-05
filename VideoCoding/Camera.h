#pragma once

#include <vector>

class Camera
{
public:
	static void test();
	static void startCam();
	static std::vector<unsigned char> Camera::getFrame();
};
