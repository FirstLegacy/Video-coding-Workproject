#include "stdafx.h"
#include "Camera.h"

#include <opencv.hpp>

cv::VideoCapture cap;

void Camera::test() {
	cv::VideoCapture capt;
	
	if (!capt.open(0))
		return;

	capt.set(CV_CAP_PROP_FRAME_WIDTH, img_res_w);
	capt.set(CV_CAP_PROP_FRAME_HEIGHT, img_res_h);

	cv::Mat frame;

	for (;;)
	{
		capt >> frame;
		if (frame.empty()) break; // end of video stream
		cv::imshow("this is you, smile! :)", frame);
		if (cv::waitKey(10) == 27) break; // stop capturing by pressing ESC
	}
}

void Camera::startCam() {
	if (!cap.open(0))
		return;

	cap.set(CV_CAP_PROP_FRAME_WIDTH, img_res_w);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, img_res_h);
}

std::vector<unsigned char> Camera::getFrame() {
	cv::Mat frame;
	cap >> frame;
		
	std::vector<unsigned char> image;

	image.assign(frame.datastart, frame.dataend);

	return image;
}