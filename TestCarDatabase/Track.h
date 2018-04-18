#pragma once
#include <opencv.hpp>

// CTrack dialog

class CTrack : public CDialogEx
{
	DECLARE_DYNAMIC(CTrack)

public:
	CTrack(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTrack();

// Dialog Data
	enum { IDD = IDD_TRACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	 CString car_ID,car_plate,car_logo,car_type,car_colour,car_time,car_locate;  //编号，车牌、车标、车型、颜色、时间、地点(在主对话框中用到，为public)
	 cv::Mat car_Img,face_Img;  //车辆、驾驶员图片
	 int matchPar;   //人脸匹配参数

	afx_msg void OnBnClickedTrButton1();  //确定按钮
	void DrawPicToHDC(IplImage img, UINT ID);  //通过该函数把图片显示在picture控件中   
	afx_msg void OnOpenFace();     //打开驾驶员图片
	afx_msg void OnOpenCar();   //打开车辆图片并检测
};
