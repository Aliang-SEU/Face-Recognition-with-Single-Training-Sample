#pragma once
#include <opencv.hpp>

class CFaceAlignment
{
public:
	CFaceAlignment(void);
	~CFaceAlignment(void);
	cv::Mat Alignment(cv::Mat src,cv::Mat src_points,cv::Mat dst_points);
	cv::Mat TwoPointSimilarity(cv::Mat pts1,cv::Mat pts2);
	cv::Mat Krons(cv::Mat A,cv::Mat B);
};

