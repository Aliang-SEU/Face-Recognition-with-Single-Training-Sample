#include "StdAfx.h"
#include "FaceAlignment.h"


CFaceAlignment::CFaceAlignment(void)
{
}


CFaceAlignment::~CFaceAlignment(void)
{
}

cv::Mat CFaceAlignment::Alignment(cv::Mat src,cv::Mat src_points,cv::Mat dst_points)
{
	/*cv::Mat gray_src;
	//灰度化
	if (src.channels()==3)
		cvtColor(src,gray_src, CV_BGR2GRAY);  
	else
		gray_src=src;*/
	//原图像转化为64FC3
	//目标图片
	cv::Mat dst = cv::Mat::zeros(100,100, CV_64FC3);
	//求取透视变换矩阵
	cv::Mat transform= TwoPointSimilarity(dst_points,src_points);
	transform=transform.t().inv().t();

	//对源图像进行透视变换
	warpPerspective(src,dst,transform,dst.size());
	return dst;
}

cv::Mat CFaceAlignment::TwoPointSimilarity(cv::Mat pts1,cv::Mat pts2)
{
	cv::Mat delta=cv::Mat::zeros( 2,2, CV_64FC1 );
	delta.at<double>(0,1)=-1;
	delta.at<double>(1,0)=1;

	cv::Mat pts1_n=pts1.t();
	cv::Mat temp1=(pts1.col(0)+delta*(pts1.col(1)-pts1.col(0))).t();
	pts1_n.push_back(temp1);
	pts1_n=pts1_n.t();

	cv::Mat pts2_n=pts2.t();
	cv::Mat temp2=(pts2.col(0)+delta*(pts2.col(1)-pts2.col(0))).t();
	pts2_n.push_back(temp2);
	pts2_n=pts2_n.t();

	cv::Mat temp3=cv::Mat::ones( 1,3, CV_64FC1 );
	pts1_n.push_back(temp3);

	cv::Mat temp4=cv::Mat::zeros( 2,2, CV_64FC1 );
	temp4.at<double>(0,0)=1;
	temp4.at<double>(1,1)=1;

	cv::Mat D=Krons(temp4,pts1_n.t());

	cv::Mat temp5=pts2_n.row(0).t();
	cv::Mat temp6=pts2_n.row(1).t();
	temp5.push_back(temp6);
	cv::Mat A_tr_vec=D.inv()*temp5;

	cv::Mat A=A_tr_vec.rowRange(0,3).t();
	cv::Mat temp7=A_tr_vec.rowRange(3,6).t();
	A.push_back(temp7);

	cv::Mat temp8=cv::Mat::zeros( 1,3, CV_64FC1 );
	temp8.at<double>(0,2)=1;
	A.push_back(temp8);

	return A;
}

cv::Mat CFaceAlignment::Krons(cv::Mat A,cv::Mat B)
{
	cv::Mat K=cv::Mat::zeros( 6,6, CV_64FC1 );
	K.at<double>(0,0)=A.at<double>(0,0)*B.at<double>(0,0);
	K.at<double>(0,1)=A.at<double>(0,0)*B.at<double>(0,1);
	K.at<double>(0,2)=A.at<double>(0,0)*B.at<double>(0,2);
	K.at<double>(1,0)=A.at<double>(0,0)*B.at<double>(1,0);
	K.at<double>(1,1)=A.at<double>(0,0)*B.at<double>(1,1);
	K.at<double>(1,2)=A.at<double>(0,0)*B.at<double>(1,2);
	K.at<double>(2,0)=A.at<double>(0,0)*B.at<double>(2,0);
	K.at<double>(2,1)=A.at<double>(0,0)*B.at<double>(2,1);
	K.at<double>(2,2)=A.at<double>(0,0)*B.at<double>(2,2);

	K.at<double>(0,3)=A.at<double>(0,1)*B.at<double>(0,0);
	K.at<double>(0,4)=A.at<double>(0,1)*B.at<double>(0,1);
	K.at<double>(0,5)=A.at<double>(0,1)*B.at<double>(0,2);
	K.at<double>(1,3)=A.at<double>(0,1)*B.at<double>(1,0);
	K.at<double>(1,4)=A.at<double>(0,1)*B.at<double>(1,1);
	K.at<double>(1,5)=A.at<double>(0,1)*B.at<double>(1,2);
	K.at<double>(2,3)=A.at<double>(0,1)*B.at<double>(2,0);
	K.at<double>(2,4)=A.at<double>(0,1)*B.at<double>(2,1);
	K.at<double>(2,5)=A.at<double>(0,1)*B.at<double>(2,2);

	K.at<double>(3,0)=A.at<double>(1,0)*B.at<double>(0,0);
	K.at<double>(3,1)=A.at<double>(1,0)*B.at<double>(0,1);
	K.at<double>(3,2)=A.at<double>(1,0)*B.at<double>(0,2);
	K.at<double>(4,0)=A.at<double>(1,0)*B.at<double>(1,0);
	K.at<double>(4,1)=A.at<double>(1,0)*B.at<double>(1,1);
	K.at<double>(4,2)=A.at<double>(1,0)*B.at<double>(1,2);
	K.at<double>(5,0)=A.at<double>(1,0)*B.at<double>(2,0);
	K.at<double>(5,1)=A.at<double>(1,0)*B.at<double>(2,1);
	K.at<double>(5,2)=A.at<double>(1,0)*B.at<double>(2,2);

	K.at<double>(3,3)=A.at<double>(1,1)*B.at<double>(0,0);
	K.at<double>(3,4)=A.at<double>(1,1)*B.at<double>(0,1);
	K.at<double>(3,5)=A.at<double>(1,1)*B.at<double>(0,2);
	K.at<double>(4,3)=A.at<double>(1,1)*B.at<double>(1,0);
	K.at<double>(4,4)=A.at<double>(1,1)*B.at<double>(1,1);
	K.at<double>(4,5)=A.at<double>(1,1)*B.at<double>(1,2);
	K.at<double>(5,3)=A.at<double>(1,1)*B.at<double>(2,0);
	K.at<double>(5,4)=A.at<double>(1,1)*B.at<double>(2,1);
	K.at<double>(5,5)=A.at<double>(1,1)*B.at<double>(2,2);

	return K;

}