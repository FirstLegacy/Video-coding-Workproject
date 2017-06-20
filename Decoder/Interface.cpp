#include "stdafx.h"
#include "Interface.h"

#include <thread>
#include <opencv.hpp>

void Interface::update(std::vector<unsigned char> new_frame) {
	cv::Mat image(img_res_h, img_res_w, CV_8UC3, new_frame.data());

	cv::imshow("Display window", image);
	if (cv::waitKey(10) == 27) return; // stop capturing by pressing ESC
}

void Interface::init() {
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);// Create a window for display.

	std::cout << "Interface started." << std::endl;
}