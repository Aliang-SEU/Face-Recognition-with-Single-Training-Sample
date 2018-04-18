#pragma once
#include"opencv.hpp"

int  max_rate(std::vector<float>& rate);
class FaceDetect2
{
public:
	FaceDetect2();
	~FaceDetect2();
	cv::Rect draw(cv::Mat);
	float skin_face(cv::Mat likeface);
	cv::Mat face_Image,car_Image;
	cv::CascadeClassifier pcar_cascade;
	cv::CascadeClassifier pface_cascade;
};

