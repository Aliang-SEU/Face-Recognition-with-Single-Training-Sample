#pragma once
#include <opencv.hpp>

// CInputOne dialog

class CInputOne : public CDialogEx
{
	DECLARE_DYNAMIC(CInputOne)

public:
	CInputOne(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputOne();

// Dialog Data
	enum { IDD = IDD_INPUTONE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString DataSourse,UseName,PassWord;  //连接数据库的数据源、用户名、密码（在主对话框中用到，所以为public）

private:
	_ConnectionPtr m_pConn;//数据库连接指针
	_RecordsetPtr m_pRs,m_pRs1;//记录集指针

	CString car_plate,car_logo,car_type,car_colour,car_locate,car_time;;  //车牌等信息

	cv::Mat carImage;//原图  注意：cv::Mat_<float> carImage，定义成这样时，用imread报错

public:
	afx_msg void OnOpenImage();  //打开图片
	afx_msg void OnInputData();   //录入数据
	void DrawPicToHDC(IplImage img, UINT ID);  //通过该函数把图片显示在picture控件中
};
