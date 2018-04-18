#include "StdAfx.h"
#include "Wavelet.h"


Wavelet::Wavelet(void)
{
}


Wavelet::~Wavelet(void)
{
}

void Wavelet::get_wavelet_img(cv::Mat src)
{
	wavelet_denoising(src, "haar", 1, 1, R, L);
	L = normalize8(L, 1);
	R = normalize8(R, 1);

	this->L=L;
	this->R=R;
}

void Wavelet::wavelet_denoising(cv::Mat &src, std::string wname, int level, int normalize , cv::Mat &R, cv::Mat &L)
{
	cv::Mat temp_src(src.rows, src.cols, CV_64F), save_src(src.rows, src.cols, CV_64F);
	cv::Mat dst(src.rows, src.cols, CV_64F);

	cv::Mat CA[3], CH[3], CV[3], CD[3]; 
	
	src.convertTo(temp_src,CV_64F);
	cv::Size size = src.size();

	//转换到对数域	M1=log(1+M)  (调试完成)
	temp_src = normalize8(src, 1);
	temp_src = temp_src + cv::Scalar::all(1);
	log(temp_src, temp_src);
	temp_src = normalize8(temp_src, 1);
	//cout << temp_src << endl;
	//保存原图
	save_src = temp_src.clone();
	//小波变换，进行非线性阈值去噪
	for (int i = 0; i < level;++i)
	{
		dst = WaveletTransformHaar(temp_src, CA[i], CH[i], CV[i], CD[i]);
		cv::Mat kk = thresholding(CH[i], CD[i], CA[i]);
		CV[i] = thresholding(CV[i], CD[i], CA[i]);
		CD[i] = thresholding(CD[i], CD[i], CA[i]);
		CA[i] = CA[i];
		temp_src = CA[i];
	}
	temp_src = CA[level-1];
	//小波逆变换
	for (int i = level - 1; i>=0; --i)
	{
		cv::Size _size = CH[i].size(); // 非必要
		resize(temp_src, temp_src, _size);
		temp_src=InvertWaveletTransformHaar(temp_src, CH[i], CV[i], CD[i]);
	}
	//分离图像
	R = save_src - temp_src;
	L = temp_src;

	//后期处理
	if (normalize != 0)
	{
	//	R = normalize8(histtruncate(R, 0.2, 0.2));
	//	L = normalize8(L);
	}
}

cv::Mat Wavelet::histtruncate(cv::Mat src, double lHistCut, double uHistCut)
{
	cv::Mat_<double> dst(src.size());
	src = normalize8(src, 0);
	cv::Size size = src.size();
	int m = size.height*size.width;
	cv::Mat_<double> sortv;
	cv::Mat_<double> temp_src = src.reshape(0, m);
	cv::sort(temp_src, sortv, CV_SORT_EVERY_COLUMN | CV_SORT_ASCENDING);
	
	cv::Mat newele;
	newele.create(m+2, 1, CV_64F);
	for (int i = 0; i < m; ++i)
	{
		newele.at<double>(i+1, 0) = sortv.at<double>(i, 0);
	}
	newele.at<double>(0, 0) = sortv.at<double>(0, 0);
	newele.at<double>(m + 1, 0) = sortv.at<double>(m-1, 0);
	//cout << newele.at<double>(m -1, 0) << endl;

	cv::Mat x;
	x.create(1,m+2, CV_64F);
	double *p = x.ptr<double>(0);
	for (int i = 1; i < m+1; ++i)
	{
		*(p + i) = 100 * ( i * 1 - 0.5) / m;
	}
	*p = 0; *(p + m + 1) = 100;
	double n1 = interp1(x, newele, newele.rows, lHistCut );
	double n2 = interp1(x, newele, newele.rows, 100 - uHistCut );
	ImageAdjust(src, dst, n1, n2, 0, 1, 1);
	return dst;
}
/**************线性插值***********
(x0,y0)->(x1,y1)
*********************************/
double inter_linear(double x0, double x1, double y0, double y1, double x)
{
	double a0, a1, y;
	a0 = (x - x1) / (x0 - x1);
	a1 = (x - x0) / (x1 - x0);
	y = a0*y0 + a1*y1;

	return (y);
}
/**************一维线性插值*************
x,y为长度为n的向量
***************************************/
double Wavelet::interp1(cv::Mat x, cv::Mat y, int n, double pp)
{
	double  z;
	int i, tmp;
	for (i = 0; i<n; i++)
	{
		if ((pp >= x.at<double>(0, i)) && (pp < x.at<double>(0, i+1)))
		{
			tmp = i;
			break;
		}
	}
	if (i == n)
	{
		z = y.at<double>(i - 1,0);
		return z;
	}
	z = inter_linear(x.at<double>(0, i), x.at<double>(0, i + 1), y.at<double>(i, 0), y.at<double>(i + 1, 0), pp);

	return z;
}
//图像对比度增强
//输入参数:src 源图像
//	   dst 目标图像
//         low 输入图像最小值 high输入图像最大值
//	   bottom 输入图像最小值 top输出图像最大值
//	   gamma 亮度附加值		
int Wavelet::ImageAdjust(cv::Mat src, cv::Mat dst, double low, double high, double bottom, double top, double gamma)
{
	if (low<0 && low>1 && high <0 && high>1 && bottom<0 && bottom>1 && top<0 && top>1 && low>high)
		return -1;
	double low2 = low * 1;
	double high2 = high * 1;
	double bottom2 = bottom * 1;
	double top2 = top * 1;
	double err_in = high2 - low2;
	double err_out = top2 - bottom2;
	int x, y;
	double val;
	for (y = 0; y < src.rows; y++)
	{
		double *p = src.ptr<double>(y);
		double *q = dst.ptr<double>(y);
		for (x = 0; x < src.cols; x++)
		{
			val = *(p+x);
			val = pow((val - low2) / err_in, gamma) * err_out + bottom2;
			if (val>1) val = 1; if (val<0) val = 0; // Make sure src is in the range [low,high]
			*(q+x) = (double)val;
		}
	}
	return 0;

}
//函数功能:	非线性阈值(调试完成)
//输入:	Mat &X, Mat &HH,Mat Z
//输出:	Mat
cv::Mat Wavelet::thresholding(cv::Mat X, cv::Mat HH,cv::Mat Z)
{
	cv::Mat Y = cv::Mat::zeros(X.rows, X.cols, CV_64F);	//构造0矩阵
	HH = X.clone();
	double T = computeT(X, HH);						//计算阈值
	//cout << T<<endl;
	//非线性阈值修正
	for (int i = 0; i < X.rows; ++i)
	{
		double *p = X.ptr<double>(i);
		double *q = Y.ptr<double>(i);

		for (int j = 0; j < X.cols*X.channels(); ++j)	//
		{
			if (*(p + j) >= T)
			{
				*(q + j) = *(p + j) - T;
			}
			else if (*(p + j) <= -T)
			{
				*(q + j) = *(p + j) + T;
			}
			else
			{
				*(q + j) = 0;
			}
		}
	}
	return Y;
}

//函数功能: 计算均方差(调试完成)
//输入:	Mat &X
//输出:	double 
double Wavelet::mad(cv::Mat &X)
{
	double sum = 0.0f, ave = 0.0f;
	for (int i = 0; i < X.rows; ++i)
	{
		double* p = X.ptr<double>(i);
		for (int j = 0; j < X.cols*X.channels(); ++j)
		{
			sum += *(p + j);
		}
	}
	ave = sum / (X.rows*X.cols*X.channels());
	sum = 0.0f;
	for (int i = 0; i < X.rows; ++i)
	{
		double* p = X.ptr<double>(i);
		for (int j = 0; j < X.cols*X.channels(); ++j)
		{
			sum += abs(*(p + j) - ave);
		}
	}
	ave = sum / (X.rows*X.cols*X.channels());
	return ave;
}

//函数功能: 计算方差(调试完成)
//输入:	Mat &X
//输出:	double 
double Wavelet::var(cv::Mat &X)
{
	double sum = 0.0f, ave = 0.0f;
	for (int i = 0; i < X.rows; ++i)
	{
		double* p = X.ptr<double>(i);
		for (int j = 0; j < X.cols*X.channels(); ++j)
		{
			sum += *(p + j);
		}
	}
	ave = sum / (X.rows*X.cols*X.channels());
	sum = 0.0f;
	for (int i = 0; i < X.rows; ++i)
	{
		double* p = X.ptr<double>(i);
		for (int j = 0; j < X.cols*X.channels(); ++j)
		{
			sum += pow(*(p + j) - ave, 2);
		}
	}
	ave = sum / (X.rows*X.cols*X.channels());
	return ave;
}
//函数功能: 计算实际的阈值(调试完成)
//输入:	Mat &X,Mat &HH
//输出:	double T
double Wavelet::computeT(cv::Mat &X, cv::Mat &HH)
{
	double lambda = 0.1;
	double sigma = 0.0f, sigma_x = 0.0f, sigma_y = 0.0f;
	double T11, T22;
	double T1;
	sigma = mad(HH) / lambda;	//扩大十倍?????
	sigma_y = var(X);
	sigma_x = sqrt(cv::max((double) (pow(sigma_y, 2) - pow(sigma, 2)), 0.0));
	if (sigma_x == 0)
	{
		minMaxLoc(abs(X), &T22, &T11);
		T1 = (double)T11;
	
	}
	else
	{
		T1 = pow(sigma, 2) / sigma_x;
	}
	return T1;
}


//haar二维小波变换(测试完成)
cv::Mat Wavelet::WaveletTransformHaar(const cv::Mat &_src, cv::Mat &CA, cv::Mat &CH, cv::Mat &CV, cv::Mat &CD)
{
	cv::Mat src = _src.clone();
	cv::Size size = _src.size();

	//行变换
	for (int i = 0; i < size.height; ++i)
	{
		//取出要计算的那行
		cv::Mat oneRow = cv::Mat::zeros(1, size.width, src.type());
		double *p = oneRow.ptr<double>(0);
		double *q = src.ptr<double>(i);
		for (int j = 0; j<size.width; ++j)
		{
			*(p + j) = *(q + j);
			//	oneRow.at<double>(0, j) = src.at<double>(i, j);
		}
		DecomposeWave(oneRow);
		// 将src这一行置为oneRow中的数据     
		p = oneRow.ptr<double>(0);
		q = src.ptr<double>(i);
		for (int j = 0; j<size.width; ++j)
		{
			*(q + j) = *(p + j);
			//src.at<double>(i, j) = oneRow.at<double>(0, j);
		}
	}
	for (int j = 0; j < size.width; ++j)
	{
		//取出src数据的一行输入
		cv::Mat oneCol = cv::Mat::zeros(size.height, 1, src.type());
		for (int i = 0; i < size.height; ++i)
		{
			oneCol.at<double>(i, 0) = src.at<double>(i, j);
		}
		DecomposeWave(oneCol);
		for (int i = 0; i < size.height; ++i)
		{
			src.at<double>(i, j) = oneCol.at<double>(i, 0);
		}
	}

	CA = src(cv::Rect(0, 0, src.cols / 2, src.rows / 2));
	CV = src(cv::Rect(src.cols / 2,0, src.cols / 2, src.rows / 2));
	CH = src(cv::Rect(0, src.rows/2, src.cols / 2, src.rows / 2));
	CD = src(cv::Rect(src.cols / 2, src.rows / 2, src.cols/2 , src.rows/2 ));
	return src;
}
void Wavelet::DecomposeWave(cv::Mat &src)
{
	cv::Mat temp_src(src.rows, src.cols, src.type());
	int D = src.cols;
	int W = src.rows;

	if (src.rows == 1)
	{
		assert(D % 2 == 0);
		D = D / 2;
		double *p = temp_src.ptr<double>(0);
		double *q = src.ptr<double>(0);
		for (int i = 0; i < D; ++i)
		{
			*(p + i) = (*(q + 2 * i) + *(q + 2 * i + 1)) / sqrtf(2);
			*(p + i + D) = (*(q + 2 * i) - *(q + 2 * i + 1)) / sqrtf(2);
			//temp_src.at<double>(0, i) = (src.at<double>(0, 2 * i) + src.at<double>(0, 2 * i + 1)) / sqrtf(2);
			//temp_src.at<double>(0, i + D) = (src.at<double>(0, 2 * i) - src.at<double>(0, 2 * i + 1)) / sqrtf(2);
		}
	}
	else
	{
		W = W / 2;
		for (int j = 0; j < W; ++j)
		{
			temp_src.at<double>(j, 0) = (src.at<double>(2 * j,0) + src.at<double>(2 * j + 1,0)) / sqrtf(2);
			temp_src.at<double>(j + W, 0) = (src.at<double>(2 * j, 0) - src.at<double>(2 * j + 1, 0)) / sqrtf(2);
		}
	}
	src = temp_src.clone();
}


cv::Mat Wavelet::InvertWaveletTransformHaar(cv::Mat &CA, cv::Mat &CH, cv::Mat &CV, cv::Mat &CD)
{
	cv::Mat src(CA.rows * 2, CA.cols * 2, CA.type());

	for (int j = 0; j < src.cols; ++j)
	{
		cv::Mat oneCol = cv::Mat::zeros(src.rows, 1, src.type());
		//取出该列
		if (j < CA.cols)
		{
			for (int i = 0; i < src.rows; ++i)
			{
				if (i < CA.rows)
				{
					oneCol.at<double>(i, 0) = CA.at<double>(i, j);
				}
				else
				{
					oneCol.at<double>(i, 0) = CV.at<double>(i - CA.rows, j);
				}
			}
		}
		else
		{
			for (int i = 0; i < src.rows; ++i)
			{
				if (i < CH.rows)
				{
					oneCol.at<double>(i, 0) = CH.at<double>(i, j - CA.cols);
				}
				else
				{
					oneCol.at<double>(i, 0) = CD.at<double>(i - CH.rows, j - CA.cols);
				}
			}
		}
		ComposeWave(oneCol);	//逆变换
		for (int i = 0; i < src.rows; ++i)
		{
			src.at<double>(i, j) = oneCol.at<double>(i, 0);
		}
	}
	//小波行逆变换
	for (int i = 0; i < src.rows; ++i)
	{
		cv::Mat oneRow = cv::Mat::zeros(1, src.cols, src.type());
		for (int j = 0; j < src.cols; ++j)
		{

			oneRow.at<double>(0, j) = src.at<double>(i, j);
		}
		ComposeWave(oneRow);	//逆变换
		for (int j = 0; j < src.cols; ++j)
		{
			src.at<double>(i, j) = oneRow.at<double>(0, j);
		}
	}
	return src;
}

void Wavelet::ComposeWave(cv::Mat &src)
{
	cv::Mat temp_src(src.rows, src.cols, src.type());
	int D = src.cols;
	int W = src.rows;


	if (src.rows == 1)
	{
		D = D / 2;

		for (int i = 0; i < D; ++i)
		{
			temp_src.at<double>(0, 2*i) = (src.at<double>(0, i) + src.at<double>(0, i + D)) / sqrtf(2);
			temp_src.at<double>(0, 2*i + 1) = (src.at<double>(0, i) - src.at<double>(0, i + D)) / sqrtf(2);
		}
	}
	else
	{
		W = W / 2;
		for (int j = 0; j < W; ++j)
		{
			temp_src.at<double>(2*j, 0) = (src.at<double>(j, 0) + src.at<double>(j + W, 0)) / sqrtf(2);
			temp_src.at<double>(2*j+1 , 0) = (src.at<double>(j, 0) - src.at<double>(j + W , 0)) / sqrtf(2);
		}
	}
	src = temp_src.clone();
}


//函数功能：归一化 (调试完成)
//参数说明：src 输入矩阵
//			mode 模式选择 0：归一化后的值为（0,1）
//					1：归一化后的值为（0,255） 注意：接收的Mat为浮点型时 浮点型的像素表示范围为0-1.0
//返回值：	Mat：归一化后的矩阵
cv::Mat Wavelet::normalize8(cv::Mat &src, int mode)
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
//函数功能：矩阵向上取整函数(调试完成)
//输入： 输入矩阵
//输出： 无
void Wavelet::CvMatCeil(cv::Mat &X)
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