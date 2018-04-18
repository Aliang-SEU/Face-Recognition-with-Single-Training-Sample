#pragma once
#include <opencv.hpp>

class FaceRec
{
public:
	FaceRec(void);
	~FaceRec(void);


public:
	int* recognition(cv::Mat_<double> A,cv::Mat_<double> D,cv::Mat_<double> y_AsCol,int RecNum,int nTotals);   //主函数
	int  recognition2(cv::Mat_<double> A,cv::Mat_<double> D,cv::Mat_<double> y_AsCol);   //主函数2
	int sgn(double x);   //符号函数
	cv::Mat_<double> BPDN_homotopy_function(cv::Mat_<double> A,cv::Mat_<double> y,double tolerance);  //同伦算法主函数
	//更新步长函数
	//gamma_x:支撑集；x_k：这步迭代得到的解；del_x_vec：方向向量；pk、dk、epsilon：看调用处； 后两个参数当输出参数用！i_delta：从支撑集补集跳到支撑集的那个元素的位置；out_x：移除支撑集的那个元素的位置
	double update_primal(cv::Mat_<int> gamma_x,cv::Mat_<double> x_k,cv::Mat_<double> del_x_vec,cv::Mat_<double> pk,cv::Mat_<double> dk,double epsilon,cv::Mat_<int> &i_delta,cv::Mat_<int> &out_x);
	cv::Mat_<double> update_inverse(cv::Mat_<double> AtB,cv::Mat_<double> iAtB_old,int flag);   //计算iAtgxAgx的函数
};

