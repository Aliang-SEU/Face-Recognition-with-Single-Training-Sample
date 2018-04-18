#include "stdafx.h"
#include "FaceDetect2.h"
#include"vector"
#include"algorithm"
#include"opencv.hpp"

FaceDetect2::FaceDetect2(){
}
FaceDetect2::~FaceDetect2(){}
bool compare(cv::Rect r1, cv::Rect r2){
	return (r1.height*r1.width)>(r2.height*r2.width);
}

cv::Rect FaceDetect2::draw(cv::Mat src)
{
	double scale = 3.0;
	cv::string car_cascade_name = "classifier_car.xml";
	cv::string face_cascade_name = "classifier_face.xml";
	
	std::vector<cv::Rect> cars;
	std::vector<cv::Rect> faces;
	cv::Mat gray_src;
	cv::Mat small_gray;
	cv::cvtColor(src, gray_src, CV_BGR2GRAY);
	cv::resize(gray_src, small_gray, cv::Size(0, 0), 1 / scale, 1 / scale, cv::INTER_LINEAR);
	cars.clear();
	pcar_cascade.load(car_cascade_name);
	pcar_cascade.detectMultiScale(small_gray, cars, 1.2, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(50, 50));
	if (!cars.empty())
	{
		cv::Rect car_range(0, 0, 0, 0);
		sort(cars.begin(), cars.end(), compare);
		car_range.x = (int)(cars[0].x*scale);
		car_range.y = (int)(cars[0].y*scale);
		car_range.height = (int)(cars[0].height*scale);
		car_range.width = (int)(cars[0].width*scale);
		cv::Mat color_car(src, car_range);
		car_Image = color_car.clone();
		cv::Mat gray_car;
		cv::cvtColor(color_car, gray_car, CV_BGR2GRAY);
		faces.clear();
		pface_cascade.load(face_cascade_name);
		pface_cascade.detectMultiScale(gray_car, faces, 1.02, 1, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));
		std::vector<float> rate;
		rate.clear();
		for (std::vector<cv::Rect>::iterator it = faces.begin(); it != faces.end();)
		{
			cv::Mat _like(color_car, cv::Rect((*it).x, (*it).y, (*it).width, (*it).height));
			float sure = skin_face(_like);
			if ((*it).y>(color_car.rows >> 1) || (*it).x<(color_car.cols >> 1))//人脸位置判别
				it = faces.erase(it);
			else
			{
				it++;
				rate.push_back(sure);
			}
		}

		int num = max_rate(rate);
		cv::Rect face_range;
		if (!faces.empty())
		{
			face_range.x = (int)(faces[num].x);
			face_range.y = (int)(faces[num].y);
			face_range.height = (int)(faces[num].height);
			face_range.width = (int)(faces[num].width);
		}
		else{
			face_range.x = 0;
			face_range.y = 0;
			face_range.height = 0;
			face_range.width = 0;
		}
		cv::Rect range = cv::Rect(car_range.x + face_range.x, car_range.y + face_range.y, face_range.width, face_range.height);
		cv::Mat face(src, range);
		face_Image = face.clone();
		return range;
	}
	else{
		return cv::Rect(0, 0, 0, 0);
	}
}
//内部调用函数
float FaceDetect2::skin_face(cv::Mat likeface)
{
	std::vector<cv::Mat>channels;
	cv::Mat C;
	cv::Mat RGB;
	cv::Mat YCrCb;
	cv::Mat result;
	cv::Mat inv_C;
	cv::Mat cr;
	cv::Mat cb;


	//分离Cr、Cb两个通道
	if (likeface.channels() == 3){
		RGB = likeface.clone();
	}
	else{
		cv::cvtColor(likeface, RGB, CV_GRAY2BGR);
	}
	cv::cvtColor(RGB, YCrCb, CV_BGR2YCrCb);
	channels.clear();
	cv::split(YCrCb, channels);
	cr = channels.at(1);
	cb = channels.at(2);
	//计算均值，协方差
	C = cvCreateMat(2, 2, CV_32FC1);
	C.at<float>(0, 0) = (float)97.0946;
	C.at<float>(0, 1) = (float)24.4700;
	C.at<float>(1, 0) = (float)24.4700;
	C.at<float>(1, 1) = (float)141.9966;
	inv_C = C.inv();
	result = cvCreateMat(likeface.rows, likeface.cols, CV_8UC1);
	int total_count = likeface.rows*likeface.cols;
	int count = 0;
	for (int i = 0; i < likeface.rows; i++)
	{
		for (int j = 0; j < likeface.cols; j++)
		{
			float x1 = cb.at<unsigned char>(i, j) - (float)117.4316;
			float x2 = cr.at<unsigned char>(i, j) - (float)148.5599;
			float mid = (x1*inv_C.at<float>(0, 0)*x1 + x1*inv_C.at<float>(0, 1)*x2 + x1*inv_C.at<float>(1, 0)*x2 + x2*inv_C.at<float>(1, 1)*x2)*((float)-0.5);
			if (exp(mid)>0.1)
			{
				result.at<unsigned char>(i, j) = 255;
			}
			else
			{
				result.at<unsigned char>(i, j) = 0;
			}

		}
	}

	cv::Mat element = cv::getStructuringElement(0, cv::Size(7, 7), cv::Point(3, 3));
	dilate(result, result, element);
	erode(result, result, element);
	dilate(result, result, element);
	erode(result, result, element);
	for (int i = 0; i < likeface.rows; i++)
	{
		for (int j = 0; j <likeface.cols; j++)
		{
			if (result.at<unsigned char>(i, j) == 255)
				count++;
		}
	}
	float rate = (float)count / total_count;
	return rate;
}
int  max_rate(std::vector<float>& rate)
{
	if (rate.empty())
	{
		return 0;
	}
	float max = 0;
	int mid = 0;
	for (unsigned int i = 0; i < rate.size(); i++)
	{
		if (rate[i]>max)
		{
			max = rate[i];
			mid = i;
		}

	}
	return mid;
}