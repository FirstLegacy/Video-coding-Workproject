#include "stdafx.h"
#include "Interface.h"

#include <Windows.h>
#include <thread>
#include <opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <array>

#define MS_PER_FRAME 1000/24

std::vector<unsigned char> Interface::frame;

//Input is a ras RGB file in a one frame structur
void Interface::GUI(std::vector<unsigned char> firstFrame) {
	time_t start, end, diff;
	frame = firstFrame;

	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);// Create a window for display.

	while (true) {
		time(&start);
		cv::Mat image(img_res_h, img_res_w, CV_8UC3, frame.data());

		cv::imshow("Display window", image);
		if (cv::waitKey(10) == 27) break; // stop capturing by pressing ESC

		time(&end);
		diff = difftime(start, end);
		Sleep(MS_PER_FRAME - diff);
	}
}
