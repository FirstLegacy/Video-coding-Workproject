#include "stdafx.h"
#include "Camera.h"
#include "Decoder\Interface.h"

#include <opencv.hpp>

cv::VideoCapture cap;

void Camera::startCam() {
	if (!cap.open(0))
		throw std::runtime_error("Camera not found!");

	cap.set(CV_CAP_PROP_FRAME_WIDTH, (double)img_res_w);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,(double)img_res_h);

	std::cout << "Camera started." << std::endl;
}

// Retrieves current frame from the camera.
std::vector<unsigned char> Camera::getFrame() {
	cv::Mat frame;
	cap >> frame;
		
	std::vector<unsigned char> image;

	image.assign(frame.datastart, frame.dataend);

	return image;
}