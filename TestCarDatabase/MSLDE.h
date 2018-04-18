#pragma once
#include <opencv2\opencv.hpp>


class MSLDE
{
public:
	MSLDE();
	~MSLDE();
	cv::Mat static lightProcessing(cv::Mat X, double alfa1 = 6.0f, double sigma = 1.0f, double nn = 9.0f, double alfa = 2.0f, int normalize = 0);
	cv::Mat static gaussian_kernal(int kernelSize, double sigma0);
};

