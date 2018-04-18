#pragma once
#include <opencv2\opencv.hpp>
class Retina_Model
{
public:
	Retina_Model();
	~Retina_Model();

	cv::Mat Retina_Model_Normalization(cv::Mat_<double> X, double sigma1 = 1, double sigma2 = 3, double Dogsigma1 = 0.5, double Dogsigma2 = 4, double treshold = 5, int normalize = 1);
	cv::Mat normalize8(cv::Mat &src, int mode = 1);
	void CvMatCeil(cv::Mat &X);
	cv::Mat gaussian_kernal(int kernelSize, double sigma0);
	cv::Mat DoG(cv::Mat_<double> X, double sigma1 = 0.5, double sigma2 = 4.0, int normalize = 0);
};

