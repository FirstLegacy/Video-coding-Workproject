#include "stdafx.h"
#include "Interface.h"

#include <thread>
#include <opencv.hpp>

#define MS_PER_FRAME 1000/24

/*
std::vector<unsigned char> Interface::frame(img_res_rgb);
bool Interface::updated = false;

void Interface::updateFrame(std::vector<unsigned char> new_frame) {
	frame = new_frame;
	updated = true;
}

void Interface::GUI() {
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);// Create a window for display.

	while (true) {
		if (updated) {
			cv::Mat image(img_res_h, img_res_w, CV_8UC3, frame.data());

			cv::imshow("Display window", image);
			if (cv::waitKey(10) == 27) break; // stop capturing by pressing ESC
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(MS_PER_FRAME));
	}
}
*/
void Interface::update(std::vector<unsigned char> new_frame) {
	cv::Mat image(img_res_h, img_res_w, CV_8UC3, new_frame.data());

	cv::imshow("Display window", image);
	if (cv::waitKey(10) == 27) return; // stop capturing by pressing ESC
}

void Interface::init() {
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);// Create a window for display.
}