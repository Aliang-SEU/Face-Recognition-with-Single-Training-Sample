#pragma once
#include <opencv.hpp>

// CStep dialog

class CStep : public CDialogEx
{
	DECLARE_DYNAMIC(CStep)

public:
	CStep(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStep();

// Dialog Data
	enum { IDD = IDD_STEP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()

private:
	CMenu m_Menu;   //设置菜单项
	POINT old;    //用来保存对话框的原始坐标点（改变对话框大小时用）
	cv::Mat oriImage,carImage,faceImage,feaImage,aliImage,illImage,resImage; //原图、车辆、人脸、对齐、特征点、光照、识别结果
	CvRect face_region;  //人脸图像区域
	
	CvPoint left_eye, right_eye;  //人脸坐标

public:
	afx_msg void OnReadImg();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ReSize();  //使控件随对话框大小的改变而改变
	void DrawPicToHDC(IplImage img, UINT ID);  //通过该函数把图片显示在picture控件中
	afx_msg void OnFaceDetect();
	afx_msg void OnFaceAli();
	afx_msg void OnIllumination();
	afx_msg void OnFaceRec();
	afx_msg void OnFea();
	afx_msg void OnCarDetect();
};
