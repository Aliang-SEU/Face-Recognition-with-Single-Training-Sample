#include "stdafx.h"
#include "Retina_Model.h"


Retina_Model::Retina_Model()
{
}


Retina_Model::~Retina_Model()
{
}

cv::Mat Retina_Model::normalize8(cv::Mat &src, int mode)
{
	assert(mode == 0 || mode == 1);
	cv::Size size = src.size();
	cv::Mat temp_src, dst;
	src.convertTo(temp_src, CV_64F, 1 / 255.0);	//数据转换double型 double型的数据范围为0-1.0
	dst.create(src.rows, src.cols, CV_64F);		//分配内存
	switch (mode)
	{
	case 0:normalize(temp_src, dst, 1, 0, cv::NORM_MINMAX); break;
	case 1:normalize(temp_src, dst, 255, 0, cv::NORM_MINMAX); CvMatCeil(dst); break;	//归一化 向上取整
	default:	 break;
	}
	return dst;
}
void Retina_Model::CvMatCeil(cv::Mat &X)
{
	for (int i = 0; i < X.rows; ++i)
	{
		double* p = X.ptr<double>(i);
		for (int j = 0; j < X.cols*X.channels(); ++j)
		{
			*(p + j) = cvCeil(*(p + j));
		}
	}
}

//生成二维高斯核    
//double sigma0 = (halfSize - 1)/ 2.0;  
cv::Mat Retina_Model::gaussian_kernal(int kernelSize, double sigma0)
{
	int halfSize = (kernelSize - 1) / 2;
	cv::Mat K(kernelSize, kernelSize, CV_64F);

	//生成二维高斯核    
	double s2 = 2.0 * sigma0 * sigma0;
	for (int i = (-halfSize); i <= halfSize; i++)
	{
		int m = i + halfSize;
		for (int j = (-halfSize); j <= halfSize; j++)
		{
			int n = j + halfSize;
			double v = exp(-(1.0*i*i + 1.0*j*j) / s2);
			K.ptr<double>(m)[n] = v;
		}
	}
	cv::Scalar all = sum(K);
	cv::Mat gaussK;
	K.convertTo(gaussK, CV_64F, (1 / all[0]));

	return gaussK;
}
//视网膜模型去噪
cv::Mat Retina_Model::Retina_Model_Normalization(cv::Mat_<double> X, double sigma1, double sigma2, double Dogsigma1, double Dogsigma2, double treshold, int normalize)
{
	cv::Mat Y;
	cv::Size size = X.size();
	X = normalize8(X);
#pragma region  First non-linearity
	cv::Mat Filter = gaussian_kernal((int)(2 * ceil(3 * sigma1) + 1), sigma1);
	cv::Mat F1;
	cv::Mat temp;
	X.convertTo(temp, CV_8UC1);
	filter2D(temp, F1, temp.depth(), Filter);
	cv::Scalar scalar = mean(X) / 2.0;
	F1 = F1 + scalar.val[0];						//有偏差
	F1.convertTo(F1, CV_64F);
	cv::Mat Ila1;
	double l1, l2;
	minMaxLoc(X, &l1, &l2);
	temp = X / (X + F1);
	Ila1 = (l2 + F1).mul(temp);
	Ila1 = normalize8(Ila1);
#pragma endregion

#pragma region  Second non-linearity
	Filter = gaussian_kernal((int)(2 * ceil(3 * sigma2) + 1), sigma2);
	temp = Ila1.clone();
	filter2D(temp, F1, temp.depth(), Filter);
	scalar = mean(Ila1) / 2.0;
	F1 = F1 + scalar.val[0];						//有偏差
	F1.convertTo(F1, CV_64F);
	cv::Mat Ila2;
	minMaxLoc(Ila1, &l1, &l2);
	temp = Ila1 / (Ila1 + F1);
	Ila2 = (l2 + F1).mul(temp);
#pragma endregion

#pragma region Dog filtering
	cv::Mat Ibip = DoG(Ila2, Dogsigma1, Dogsigma2, 0);
#pragma endregion

#pragma region Rescaling 
	cv::Mat Inor;
	pow(Ibip, 2, temp);
	Inor = (Ibip) / sqrt(mean(temp).val[0]);
	//	Inor = normalize8(Inor, 0);
	//	imshow("1", Inor);
#pragma endregion

#pragma region Truncation
	cv::Mat thresh_img = treshold*cv::Mat::ones(size, X.type());
	cv::Mat Ipp(size, X.type());
	for (int i = 0; i < size.height; ++i)
	{
		double *p = Inor.ptr<double>(i);
		double *q = thresh_img.ptr<double>(i);
		double *dst = Ipp.ptr<double>(i);
		for (int j = 0; j < size.width; ++j)
		{
			if (*(p + j) >= 0)
				*(dst + j) = cv::min(*(q + j), abs(*(p + j)));
			else
				*(dst + j) = -cv::min(*(q + j), abs(*(p + j)));
		}
	}
	//	Ipp = normalize8(Ipp, 0);
	//	imshow("1", Ipp);
#pragma endregion

	Y = Ipp;
//	Y = normalize8(Y,1);
	return Y;
	
}
//DoG模型
cv::Mat Retina_Model::DoG(cv::Mat_<double> X, double sigma1, double sigma2, int normalize)
{
	cv::Mat F1 = gaussian_kernal(int(2 * ceil(3 * sigma1) + 1), sigma1);
	cv::Mat F2 = gaussian_kernal(int(2 * ceil(3 * sigma2) + 1), sigma2);
	cv::Mat X1 = normalize8(X);
	cv::Mat XF1;
	filter2D(X1, XF1, X1.depth(), F1);
	cv::Mat XF2;
	filter2D(X1, XF2, X1.depth(), F2);
	cv::Mat Y = XF1 - XF2;
	return Y;
}