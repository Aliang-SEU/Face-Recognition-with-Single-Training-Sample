#include "stdafx.h"
#include "MSLDE.h"


MSLDE::MSLDE()
{
}


MSLDE::~MSLDE()
{
}

cv::Mat MSLDE::lightProcessing(cv::Mat X, double alfa1, double sigma, double nn, double alfa, int normalize)
{
	cv::Mat Y;
	
	double lambda = 1.0f;
	float al[6] = { 1.2145, 1.1392, 1.0436, 0.9478, 0.8627, 0.7923 };

	if (sqrt(nn) - ceil(sqrt(nn)) > 1e-5)
	{
		std::cout << "Error: neighborhood size \"nn\" needs to be a squared odd number in MSLDE::lightProcessing." << std::endl;
		exit(-1);
	}
	if (fmod(sqrt(nn),2)!=1)
	{
		std::cout << "Error: square root of neighborhood size \"nn\" needs to be an odd number in MSLDE::lightProcessing." << std::endl;
		exit(-1);
	}
	cv::Size size = X.size();
	cv::Mat X1;
	X.convertTo(X1, CV_64FC1);

	//转换到对数域
	X1 = X1 + cv::Scalar::all(1);
	cv::log(X1, X1);
	//cv::normalize(X1, X1, 1, 0, cv::NORM_MINMAX);

	double in_one_dim = (sqrt(nn) - 1) / 2;
	
	cv::Mat XF,F;
	F = gaussian_kernal(2*ceil(3*sigma)+1,sigma);	//二维高斯核
	filter2D(X1, XF, X1.depth(), F, cv::Point(-1, -1), 0.0f, IPL_BORDER_REPLICATE);	//卷积操作

	Y = cv::Mat::zeros(size, CV_64FC1);
	
	cv::Mat Ys[6];
	for (int i = 0; i < 6;i++)
		Ys[i] = cv::Mat::zeros(size, CV_64FC1);

	for (int k = 1; k <= alfa1; k++)
	{
		in_one_dim = k;
		cv::Mat XFP;
		copyMakeBorder(XF,XFP,k,k,k,k,IPL_BORDER_REFLECT );	//扩展边缘
		cv::Mat Ytemp = cv::Mat::zeros(size, CV_64FC1);

		cv::Scalar argument;

		for (int i = in_one_dim + 1 ; i < size.height + in_one_dim; i++)
		{
			for (int j = in_one_dim + 1 ; j < size.width + in_one_dim; j++)
			{

				argument = cv::sum(cv::sum(-(XFP(cv::Rect(j - in_one_dim - 1, i - in_one_dim - 1, 2 * in_one_dim + 1, 2 * in_one_dim + 1)) - (XFP.at<double>(i - 1, j - 1)))));
				Ytemp.at<double>(i - in_one_dim - 1, j - in_one_dim - 1) = argument[0];
			}
		}
		Ys[k - 1] = Ytemp;
	}
	

	for (int k = 1; k <= 6; k++)
	{
		if (k == 1)
			Y = Y + al[k - 1] * Ys[k - 1];
		else
			Y = Y + al[k - 1] * (Ys[k - 1] - Ys[k - 2]);
	}
	if (normalize == 1)
		cv::normalize(Y, Y, 255, 0, cv::NORM_MINMAX);
	return Y;
}
//再来一个既含有高斯核直径kernelSize，又有单独的sigma的版本：  
//double sigma0 = (halfSize - 1)/ 2.0;  
cv::Mat MSLDE::gaussian_kernal(int kernelSize, double sigma0)
{
	int halfSize = (kernelSize - 1) / 2;
	cv::Mat K(kernelSize, kernelSize, CV_64FC1);

	//生成二维高斯核    
	double s2 = 2.0 * sigma0 * sigma0;
	for (int i = (-halfSize); i <= halfSize; i++)
	{
		int m = i + halfSize;
		for (int j = (-halfSize); j <= halfSize; j++)
		{
			int n = j + halfSize;
			float v = exp(-(1.0*i*i + 1.0*j*j) / s2);
			K.ptr<double>(m)[n] = v;
		}
	}
	cv::Scalar all = sum(K);
	cv::Mat gaussK;
	K.convertTo(gaussK, CV_64FC1, (1 / all[0]));


	return gaussK;
}