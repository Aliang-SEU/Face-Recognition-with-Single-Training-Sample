#pragma once
#include <opencv.hpp>

// CConstant dialog

class CConstant : public CDialogEx
{
	DECLARE_DYNAMIC(CConstant)

public:
	CConstant(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConstant();

// Dialog Data
	enum { IDD = IDD_CONSTANT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()

private:
	CMenu m_Menu;   //设置菜单项
	POINT old;    //用来保存对话框的原始坐标点（改变对话框大小时用）
	cv::Mat oriImage,resImage; //原图、识别结果

public:
	void DrawPicToHDC(IplImage img, UINT ID);  //通过该函数把图片显示在picture控件中
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ReSize();  //使控件随对话框大小的改变而改变
	afx_msg void OnReadImage();
	afx_msg void OnFaceRec();
};
