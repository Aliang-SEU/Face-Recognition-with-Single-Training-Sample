
// TestCarDatabaseDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include <opencv.hpp>

// CTestCarDatabaseDlg dialog
class CTestCarDatabaseDlg : public CDialogEx
{
// Construction
public:
	CTestCarDatabaseDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TESTCARDATABASE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CString DataSourse,UseName,PassWord;  //连接数据库的数据源、用户名、密码（在别的类中用到，所以为public）

private:
	_ConnectionPtr m_pConn;//数据库连接指针
	_RecordsetPtr m_pRs,m_pRs1;//记录集指针
	 CString car_ID,car_plate,car_logo,car_type,car_colour,car_time,car_locate;  //编号，车牌、车标、车型、颜色、时间、地点


private:
	CMenu m_Menu;   //设置菜单项
	POINT old;    //用来保存对话框的原始坐标点（改变对话框大小时用）
	int nItem;   //列表控件的行号
	cv::Mat car_Img,face_Img,plate_Img,logo_Img;   //车辆、驾驶员、车牌、车标图片
	CString picturePath,txtPath;  //批量导入时，图片文件夹路径，txt文件路径
	int matchPar;   //人脸匹配参数
	

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ReSize();  //使控件随对话框大小的改变而改变
	CListCtrl my_ctllist1;  //列表框
	afx_msg void OnBnConDB();  //连接数据库
	afx_msg void OnShowDatabase();  //显示数据库中的记录集
	afx_msg void OnComQuery();  //组合查询
	afx_msg void OnAcQuery();  //精确套牌查询
	afx_msg void OnFuQuery();   //模糊套牌查询
	afx_msg void OnTrack();  //肇事逃逸追踪
	afx_msg void OnInputOne();  //单条录入
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);   //选定列表中的某一行
	afx_msg void OnShowImg();   //显示图片
	afx_msg void OnHideImg();   //隐藏图片
	void DrawPicToHDC(IplImage img, UINT ID);  //通过该函数把图片显示在picture控件中   
	afx_msg void OnDeleteData();  //删除指定记录
	afx_msg void OnUpdataData();  //更新数据
	afx_msg void OnInputMulti();  //多条录入
	afx_msg void OnStep();
	afx_msg void OnConstant();
};   
