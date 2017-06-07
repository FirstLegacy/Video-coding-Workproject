#include "stdafx.h"

#include <vector>

#include "rgbToYCbCr.h"
#include "Camera.h"
#include "quantize.h"
#include "socket.h"
#include <thread>
#include <ctime>
#include <chrono>


int main() {
	//Camera::test();

	Camera::startCam();

	Socket::connect();

	timer_start(runner, 1000 / 24);

}


void runner() {

	frameCount++;

	std::vector<unsigned char> image = Camera::getFrame();

	Quantize::setQuality(1);

	auto coded_img = RgbToYCbCr::convert(image);

	auto conv = coded_img;

	Socket::send(coded_img);


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