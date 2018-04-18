#pragma once
#include <opencv2\opencv.hpp>
class Wavelet
{
public:
	Wavelet(void);
	~Wavelet(void);

	void get_wavelet_img(cv::Mat src);
	void wavelet_denoising(cv::Mat &src, std::string wname, int level, int normalize , cv::Mat &R, cv::Mat &L);
	cv::Mat histtruncate(cv::Mat src, double lHistCut, double uHistCut);
	double interp1(cv::Mat x, cv::Mat y, int n, double pp);
	int ImageAdjust(cv::Mat src, cv::Mat dst, double low, double high, double bottom, double top, double gamma);
	double computeT(cv::Mat &X, cv::Mat &HH);
	cv::Mat thresholding(cv::Mat X, cv::Mat HH,cv::Mat Z);
	double mad(cv::Mat &X);
	double var(cv::Mat &X);

	
	void DecomposeWave(cv::Mat &src);
	cv::Mat WaveletTransformHaar(const cv::Mat &_src, cv::Mat &CA, cv::Mat &CH, cv::Mat &CV, cv::Mat &CD);
	cv::Mat InvertWaveletTransformHaar(cv::Mat &CA, cv::Mat &CH, cv::Mat &CV, cv::Mat &CD);
	void ComposeWave(cv::Mat &src);

	cv::Mat normalize8(cv::Mat &src, int mode=1);
	void CvMatCeil(cv::Mat &X);

	cv::Mat R,L;
};

