#include "stdafx.h"
#include "Interface.h"
#include "SocketServer.h"
#include<opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <array>
#include <ctime>
#include <chrono>
#include <thread>
#include <functional>

using namespace cv;

std::clock_t start;

int interval = 1000 / 24;

int main()
{
	SocketServer::init();
	timer_start(load_image, interval);
	while (true)
		;
}

void timer_start(std::function<void(void)> func, unsigned int interval)
{
	std::thread([func, interval]()
	{
		while (true)
		{
			auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);
			func();
			std::this_thread::sleep_until(x);
		}
	}).detach();
}

void load_image() {
	start = std::clock();

}

//Input is a ras RGB file in a one frame structur
Interface::Interface(std::array<uint8_t, img_res_rgb> in){
	Mat image;
	
	namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window", image);

	
}