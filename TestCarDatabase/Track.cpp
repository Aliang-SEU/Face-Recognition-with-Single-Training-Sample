// Track.cpp : implementation file
//

#include "stdafx.h"
#include "TestCarDatabase.h"
#include "Track.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include "FaceDetect2.h"

// CTrack dialog

IMPLEMENT_DYNAMIC(CTrack, CDialogEx)

CTrack::CTrack(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrack::IDD, pParent)
{
	matchPar=0;
}

CTrack::~CTrack()
{
}

void CTrack::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTrack, CDialogEx)
	ON_BN_CLICKED(IDC_TR_BUTTON1, &CTrack::OnBnClickedTrButton1)
	ON_BN_CLICKED(IDC_TR_BUTTON3, &CTrack::OnOpenFace)
	ON_BN_CLICKED(IDC_TR_BUTTON2, &CTrack::OnOpenCar)
END_MESSAGE_MAP()


// CTrack message handlers

//确定按钮
void CTrack::OnBnClickedTrButton1()
{	
	if (face_Img.data)
	{
		matchPar=GetDlgItemInt(IDC_TR_EDIT7);

		if (matchPar>100||matchPar<0)
		{
			MessageBox("请输入准确的匹配度！");
			return;
		} 
	} 
	GetDlgItemText(IDC_TR_EDIT1,car_plate);
	GetDlgItemText(IDC_TR_EDIT2,car_logo);
	GetDlgItemText(IDC_TR_EDIT3,car_type);
	GetDlgItemText(IDC_TR_EDIT4,car_colour);
	GetDlgItemText(IDC_TR_EDIT5,car_time);
	GetDlgItemText(IDC_TR_EDIT6,car_locate);

	if (car_plate==""&&car_logo==""&&car_type==""&&car_colour==""&&car_time==""&&car_locate=="")
	{
		MessageBox("请至少输入一个文字条件！");
	} 
	else
	{
		EndDialog(1);
	}
}


//把图片显示在picture控件中
void CTrack::DrawPicToHDC(IplImage img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC= pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf( &img ); // 复制图片
	cimg.DrawToHDC( hDC, &rect ); // 将图片绘制到显示控件的指定区域内
	ReleaseDC( pDC );
}


//打开驾驶员图片
void CTrack::OnOpenFace()
{
	CFileDialog dlg(
		TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files(*.bmp;*.jpg)|*.bmp;*.jpg| All Files (*.*) |*.*||"), NULL
		);                                        // 选项图片的约定
	dlg.m_ofn.lpstrTitle = _T("选择要检测的图片");    // 打开文件对话框的标题名
	if( dlg.DoModal() != IDOK )                    // 判断是否获得图片
	{
		return;
	}

	CString mPath = dlg.GetPathName();            // 获取图片路径

	face_Img=cv::imread(mPath.GetBuffer(0),1);
	if( !face_Img.data )   // 判断是否成功载入图片
	{
		MessageBox("读入图片失败","错误提示！");
		return;
	}

	IplImage img=face_Img;    //把mat格式转为IplImage，以便在控件中显示图片
	DrawPicToHDC(img, IDC_TR_FACE_IMG); 
}


//打开车辆图片并检测
void CTrack::OnOpenCar()
{
	CFileDialog dlg(
		TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files(*.bmp;*.jpg)|*.bmp;*.jpg| All Files (*.*) |*.*||"), NULL
		);                                        // 选项图片的约定
	dlg.m_ofn.lpstrTitle = _T("选择要检测的图片");    // 打开文件对话框的标题名
	if( dlg.DoModal() != IDOK )                    // 判断是否获得图片
	{
		return;
	}

	CString mPath = dlg.GetPathName();            // 获取图片路径

	car_Img=cv::imread(mPath.GetBuffer(0),1);
	if( !car_Img.data )   // 判断是否成功载入图片
	{
		MessageBox("读入图片失败","错误提示！");
		return;
	}


	FaceDetect2 a;
	a.draw(car_Img);
	face_Img=a.face_Image;

	if (face_Img.data)
	{
		IplImage img=face_Img;    //把mat格式转为IplImage，以便在控件中显示图片
		DrawPicToHDC(img, IDC_TR_FACE_IMG); 
		IplImage img1=car_Img;    //把mat格式转为IplImage，以便在控件中显示图片
		DrawPicToHDC(img1, IDC_TR_CAR_IMG); 
	} 
	else
	{
		MessageBox("没有检测到人脸","错误提示！");
	}
}
