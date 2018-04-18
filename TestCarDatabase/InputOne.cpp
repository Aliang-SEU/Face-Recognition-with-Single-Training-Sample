// InputOne.cpp : implementation file
//
#include "stdafx.h"
#include "TestCarDatabase.h"
#include "InputOne.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include "FaceDetect.h"
#include "FaceDetect2.h"
#include "Wavelet.h"
#include "Retina_Model.h"

// CInputOne dialog

IMPLEMENT_DYNAMIC(CInputOne, CDialogEx)

CInputOne::CInputOne(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputOne::IDD, pParent)
{

}

CInputOne::~CInputOne()
{
}

void CInputOne::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInputOne, CDialogEx)
	ON_BN_CLICKED(IDC_IN_BUTTON1, &CInputOne::OnOpenImage)
	ON_BN_CLICKED(IDC_IN_BUTTON2, &CInputOne::OnInputData)
END_MESSAGE_MAP()


// CInputOne message handlers

//打开图片
void CInputOne::OnOpenImage()
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

	carImage=cv::imread(mPath.GetBuffer(0),1);
	if( !carImage.data )   // 判断是否成功载入图片
	{
		MessageBox("读入图片失败","错误提示！");
		return;
	}

	IplImage img=carImage;    //把mat格式转为IplImage，以便在控件中显示图片
	DrawPicToHDC(img, IDC_CAR_PIC); 
}


//把图片显示在picture控件中
void CInputOne::DrawPicToHDC(IplImage img, UINT ID)
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


//录入数据
void CInputOne::OnInputData()
{

	GetDlgItemText(IDC_IN_EDIT1,car_plate);
	GetDlgItemText(IDC_IN_EDIT2,car_logo);
	GetDlgItemText(IDC_IN_EDIT3,car_type);
	GetDlgItemText(IDC_IN_EDIT4,car_colour);
	GetDlgItemText(IDC_IN_EDIT5,car_time);
	GetDlgItemText(IDC_IN_EDIT6,car_locate);

	if (car_plate!=""&&car_logo!=""&&car_type!=""&&car_colour!=""&&car_time!=""&&car_locate!="")
	{
		//创建实例
		m_pConn.CreateInstance(__uuidof(Connection));    
		m_pRs.CreateInstance(__uuidof(Recordset));   
		m_pRs1.CreateInstance(__uuidof(Recordset));

		//连接指定数据库
		if(!m_pConn->State)    
		{    
			if (DataSourse!=""&&UseName!=""&&PassWord!="")
			{
				CString sql;
				sql.Format("DSN=%s;server=localhost;User ID=%s;Password=%s", DataSourse, UseName, PassWord);

				CString sInfo;  
				try    
				{    
					m_pConn->Open((_bstr_t)sql, _T(""),_T(""), adModeUnknown);    
				}catch (_com_error e)    
				{  
					sInfo.Format(_T("数据库连接失败"));  
					MessageBox(sInfo);  
				}    
			} 
			else
			{
				MessageBox("请检查数据库有无连接！");
				return;
			}
		}   

		//获取下一个car_ID
		CString sql_next="SELECT MAX(CAR_ID)  MAXID FROM hr.CARCHRCT";
		m_pRs1->Open((_bstr_t)sql_next,(IDispatch*)m_pConn,adOpenDynamic,adLockOptimistic,adCmdText);
		int nextID = 0;
		try
		{
			nextID = m_pRs1->GetCollect("MAXID");
			nextID++;
		}
		catch (_com_error e)
		{
			nextID = 1;
		}
	//特征图片（人脸、车牌、车标等）
		cv::Mat faceImageData; 

    //提取特征
		  //提取人脸特征
		if(carImage.data)
		{
			FaceDetect2 fd;
			fd.draw(carImage);
			cv::Mat faceImage = fd.face_Image;  //调用FaceDetect类的人脸检测函数
			if(faceImage.data&&faceImage.cols!=0)
			{
			    cv::resize(faceImage,faceImageData,cv::Size(150,150));
				//灰度化
				//cv::cvtColor(faceImageData,faceImageData,cv::COLOR_RGB2GRAY);
				//小波光照处理,变换之前type为0（CV_8U）变换之后为6（ CV_64F）元素范围是0---1.0,需要转换
				//Retina_Model rw;
				//faceImageData = rw.Retina_Model_Normalization(faceImageData);
				//normalize(faceImageData, faceImageData,0,255, cv::NORM_MINMAX);
			}
			else
			{
				MessageBox("没有提取到人脸特征","错误提示！");
				return;
			}
		}
		else
		{
			MessageBox("请先选择要图片","错误提示！");
			return;
		}  

       //下面还需提取车牌、车标等特征


		/*cv::imshow("", ill_faceImageData);
		cv::waitKey();*/

	// 保存车辆、人脸、车牌、车标图片到指定文件夹
		std::string car_path,face_path;  //路径
		std::ostringstream oss;
		oss<<nextID;
		car_path="CarImage\\"+oss.str()+".jpg";
		face_path="FaceImage\\"+oss.str()+".jpg";
		cv::imwrite(car_path, carImage);
		cv::imwrite(face_path, faceImageData);


		//连接指定表
		CString sql="SELECT * FROM hr.CARCHRCT";
		m_pRs->Open((_bstr_t)sql,(IDispatch*)m_pConn,adOpenDynamic,adLockOptimistic,adCmdText);

		try  
		{   
			CString sql1;
			sql1.Format("insert into hr.CARCHRCT (car_ID,car_plate,car_logo,car_type,car_colour,car_time,car_locate) values ('%d','%s','%s','%s','%s','%s','%s')",nextID, car_plate,car_logo,car_type,car_colour,car_time,car_locate);
			m_pConn->Execute((_bstr_t)sql1,NULL,adExecuteNoRecords);  
			MessageBox("添加成功！","提示");  
		}  
		catch(_com_error e)  
		{  
			MessageBox("添加失败！","提示"); 
		}  
	} 
	else
	{
		MessageBox("输入条件不能为空！");
	}
	
}
