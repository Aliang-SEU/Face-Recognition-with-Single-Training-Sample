// Step.cpp : implementation file
//

#include "stdafx.h"
#include "TestCarDatabase.h"
#include "Step.h"
#include "afxdialogex.h"
#include "CvvImage.h"
#include "FaceDetect2.h"
#include "FaceDetect.h"
#include "FaceAlignment.h"
#include "Retina_Model.h"
#include "Wavelet.h"
#include "FaceRec.h"
#include <memory>
#include "tiny_cnn.h"

using namespace tiny_cnn;
using namespace tiny_cnn::activation;


IMPLEMENT_DYNAMIC(CStep, CDialogEx)

CStep::CStep(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStep::IDD, pParent)
{

}

CStep::~CStep()
{
}

void CStep::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	CRect temprect(0,0,960,640); 
	CWnd::SetWindowPos(NULL,0,0,temprect.Width(),temprect.Height(),SWP_NOZORDER|SWP_NOMOVE);

	//添加菜单
	m_Menu.LoadMenu(IDR_STEP_MENU);  //  IDR_MENU1为你加入的菜单的ID，在Resource视图的Menu文件夹下可以找到
	SetMenu(&m_Menu);


	//设置静态文本框字体大小
	CFont * f; 
	f = new CFont; 
	f->CreateFont(20,            // nHeight 
		0,           // nWidth 
		0,           // nEscapement 
		0,           // nOrientation 
		FW_SEMIBOLD,     // nWeight 
		FALSE,        // bItalic 
		FALSE,       // bUnderline 
		0,           // cStrikeOut 
		ANSI_CHARSET,              // nCharSet 
		OUT_DEFAULT_PRECIS,        // nOutPrecision 
		CLIP_DEFAULT_PRECIS,       // nClipPrecision 
		DEFAULT_QUALITY,           // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("宋体"));              // lpszFac

	GetDlgItem(IDC_P_STATIC1)->SetFont(f);
	GetDlgItem(IDC_P_STATIC2)->SetFont(f);
	GetDlgItem(IDC_P_STATIC3)->SetFont(f);
	GetDlgItem(IDC_P_STATIC4)->SetFont(f);
	GetDlgItem(IDC_P_STATIC5)->SetFont(f);
	GetDlgItem(IDC_P_STATIC6)->SetFont(f);
	GetDlgItem(IDC_P_STATIC7)->SetFont(f);
}

BEGIN_MESSAGE_MAP(CStep, CDialogEx)
	ON_COMMAND(ID_32795, &CStep::OnReadImg)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_COMMAND(ID_32797, &CStep::OnFaceDetect)
	ON_COMMAND(ID_32799, &CStep::OnFaceAli)
	ON_COMMAND(ID_32800, &CStep::OnIllumination)
	ON_COMMAND(ID_32801, &CStep::OnFaceRec)
	ON_COMMAND(ID_32798, &CStep::OnFea)
	ON_COMMAND(ID_32805, &CStep::OnCarDetect)
END_MESSAGE_MAP()

void CStep::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		//对话框改变时，对picture控件上的图片进行重绘
		if(oriImage.data)
		{
			IplImage img=oriImage;    //把mat格式转为IplImage，以便在控件中显示图片
			DrawPicToHDC(img, IDC_S_ORI_IMG); 
		}
	/*	if(carImage.data)
		{
			IplImage img=carImage;    //把mat格式转为IplImage，以便在控件中显示图片
			DrawPicToHDC(img, IDC_S_CAR_IMG); 
		}*/
		if(faceImage.data)
		{
			IplImage img=faceImage;    //把mat格式转为IplImage，以便在控件中显示图片
			DrawPicToHDC(img, IDC_S_FACE_IMG); 
		}
		if(aliImage.data)
		{
			IplImage img=aliImage;    //把mat格式转为IplImage，以便在控件中显示图片
			DrawPicToHDC(img, IDC_S_ALI_IMG); 
		}
		if(feaImage.data)
		{
			IplImage img=feaImage;    //把mat格式转为IplImage，以便在控件中显示图片
			DrawPicToHDC(img, IDC_S_FEA_IMG); 
		}
		if(illImage.data)
		{
			IplImage img=illImage;    //把mat格式转为IplImage，以便在控件中显示图片
			DrawPicToHDC(img, IDC_S_ILL_IMG2); 
		}
		if(resImage.data)
		{
			IplImage img=resImage;    //把mat格式转为IplImage，以便在控件中显示图片
			DrawPicToHDC(img, IDC_S_RES_IMG); 
		}
	}
}

void CStep::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	//控件随对话框大小而改变
	if (nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)  
	{  
		ReSize();  
	}  
}


//使控件随对话框大小的改变而改变
void CStep::ReSize()  
{  
	float fsp[2];  
	POINT Newp; //获取现在对话框的大小  
	CRect recta;      
	GetClientRect(&recta);     //取客户区大小    
	Newp.x=recta.right-recta.left;  
	Newp.y=recta.bottom-recta.top;  
	fsp[0]=(float)Newp.x/old.x;  
	fsp[1]=(float)Newp.y/old.y;  
	CRect Rect;  
	int woc;  
	CPoint OldTLPoint,TLPoint; //左上角  
	CPoint OldBRPoint,BRPoint; //右下角  
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件    
	while(hwndChild)      
	{      
		woc=::GetDlgCtrlID(hwndChild);//取得ID  
		GetDlgItem(woc)->GetWindowRect(Rect);    
		ScreenToClient(Rect);    
		OldTLPoint = Rect.TopLeft();    
		TLPoint.x = long(OldTLPoint.x*fsp[0]);    
		TLPoint.y = long(OldTLPoint.y*fsp[1]);    
		OldBRPoint = Rect.BottomRight();    
		BRPoint.x = long(OldBRPoint.x *fsp[0]);    
		BRPoint.y = long(OldBRPoint.y *fsp[1]);    
		Rect.SetRect(TLPoint,BRPoint);    
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);  
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);      
	}  
	old=Newp;  
}  


//把图片显示在picture控件中
void CStep::DrawPicToHDC(IplImage img, UINT ID)
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


//读入图像
void CStep::OnReadImg()
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

	oriImage=cv::imread(mPath.GetBuffer(0),1);

	if( !oriImage.data )   // 判断是否成功载入图片
	{
		MessageBox("读入图片失败","错误提示！");
		return;
	}

	IplImage img = oriImage;    //把mat格式转为IplImage，以便在控件中显示图片
	DrawPicToHDC(img, IDC_S_ORI_IMG); 
}

//人脸检测
void CStep::OnFaceDetect()
{
	if (!carImage.data)   // 判断是否成功载入图片
	{
		MessageBox("请先进行车辆检测！", "错误提示！");
		return;
	}

	FaceDetect2 fd;
	cv::Rect face_range =fd.draw(oriImage);
	faceImage = fd.face_Image;
	cv::imwrite("1.jpg",faceImage);
	IplImage img=faceImage;    //把mat格式转为IplImage，以便在控件中显示图片
	DrawPicToHDC(img, IDC_S_FACE_IMG); 
	
	face_region.x = face_range.x;
	face_region.y = face_range.y;
	face_region.width = face_range.width;
	face_region.height = face_range.height;
}

void CStep::OnFea()
{
	if (!faceImage.data)   // 判断是否成功载入图片
	{
		MessageBox("请先进行人脸检测！", "错误提示！");
		return;
	}

	// construct LeNet-5 architecture
	network<mse, gradient_descent_levenberg_marquardt> nn;

	// connection table [Y.Lecun, 1998 Table.1]
#define O true
#define X false
	static const bool connection[] = {
		O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
		O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
		O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
		X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
		X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
		X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
	};
#undef O
#undef X

	nn << convolutional_layer<tan_h>(32, 32, 5, 1, 6) // 32x32 in, 5x5 kernel, 1-6 fmaps conv
		<< average_pooling_layer<tan_h>(28, 28, 6, 2) // 28x28 in, 6 fmaps, 2x2 subsampling
		<< convolutional_layer<tan_h>(14, 14, 5, 6, 16,
		connection_table(connection, 6, 16)) // with connection-table
		<< average_pooling_layer<tan_h>(10, 10, 16, 2)
		<< convolutional_layer<tan_h>(5, 5, 5, 16, 120)
		<< fully_connected_layer<tan_h>(120, 10);

	//std::cout << "load models..." << std::endl;

	std::string testpath = "1.jpg";  //  测试图片位置

	// load MNIST dataset
	std::vector<vec_t> train_labels, test_labels;
	std::vector<vec_t> train_images, test_images;
	std::vector<vec_t> train_faceAreas, test_faceAreas;
	std::vector<std::string> train_imgNames, test_imgNames;

	//getImageAndLabel("trainImageList.txt", &train_images, &train_labels, &train_faceAreas, &train_imgNames);
	//getImageAndLabel("test.txt", &test_images, &test_labels, &test_faceAreas, &test_imgNames);

	cv::Mat gray_face;
	cv::cvtColor(faceImage, gray_face, CV_BGR2GRAY);
	getTestImage(testpath, &test_images, &test_faceAreas);

	/*/
	std::cout << "start learning" << std::endl;

	progress_display disp(train_images.size());
	timer t;
	int minibatch_size = 10;

	nn.optimizer().alpha *= std::sqrt((double)minibatch_size);

	// create callback
	auto on_enumerate_epoch = [&](){
	std::cout << t.elapsed() << "s elapsed." << std::endl;

	//tiny_cnn::result res = nn.test(test_images, test_labels);

	//std::cout << nn.optimizer().alpha << "," << res.num_success << "/" << res.num_total << std::endl;

	nn.optimizer().alpha *= 0.85; // decay learning rate
	nn.optimizer().alpha = std::max((tiny_cnn::float_t)0.00001, nn.optimizer().alpha);

	disp.restart(train_images.size());
	t.restart();
	};

	auto on_enumerate_minibatch = [&](){
	disp += minibatch_size;
	};
	/*/

	// training
	//nn.train(train_images, train_labels, minibatch_size, 1000, on_enumerate_minibatch, on_enumerate_epoch);

	//	std::cout << "end training." << std::endl;

	// save networks
	//std::ofstream ows("Weights.txt");
	//ows << nn;
	//std::cin.get();
	std::ifstream iws("Weights.txt");
	iws >> nn;
	// test and show results

	double eyes[4] = { 0, 0, 0, 0 }; //  存放网络输出的人眼坐标
	//nn.test(test_images, test_labels).print_detail(std::cout);
	//nn.test(test_images, test_labels, test_faceAreas, test_imgNames);
	nn.test(test_images, test_faceAreas, eyes);

    feaImage = faceImage.clone();
	//IplImage* showImage;
	//*showImage=IplImage(face_show);
	left_eye = cvPoint(eyes[0], eyes[1]);
	right_eye = cvPoint(eyes[2], eyes[3]);
	cv::circle(feaImage, left_eye, 1, cvScalar(0, 0, 255), 1, 8);
	cv::circle(feaImage, right_eye, 1, cvScalar(0, 0, 255), 1, 8);

	left_eye.x += face_region.x;
	left_eye.y += face_region.y;
	right_eye.x += face_region.x;
	right_eye.y += face_region.y;

	IplImage img_show = feaImage;    //把mat格式转为IplImage，以便在控件中显示图片
	DrawPicToHDC(img_show, IDC_S_FEA_IMG);

}




//人脸对齐
void CStep::OnFaceAli()
{
	if (!feaImage.data)   // 判断是否成功载入图片
	{
		MessageBox("请先进行特征点标定！", "错误提示！");
		return;
	}
	/*cv::imshow("", feaImage);
	cv::waitKey();*/
	cv::Mat src_points=cv::Mat::zeros( 2,2, CV_64FC1 );
	cv::Mat dst_points=cv::Mat::zeros( 2,2, CV_64FC1 );

	/*
	src_points.at<double>(0,0)=642.2118;
	src_points.at<double>(0,1)=682.4563;
	src_points.at<double>(1,0)=489.0939;
	src_points.at<double>(1,1)=491.6092;

	dst_points.at<double>(0,0)=10;
	dst_points.at<double>(0,1)=90;
	dst_points.at<double>(1,0)=26;
	dst_points.at<double>(1,1)=26;
	*/
	src_points.at<double>(0,0)=left_eye.x;
	src_points.at<double>(0,1)=right_eye.x;
	src_points.at<double>(1,0)=left_eye.y;
	src_points.at<double>(1,1)=right_eye.y;

	dst_points.at<double>(0,0)=30;
	dst_points.at<double>(0,1)=70;
	dst_points.at<double>(1,0)=32;
	dst_points.at<double>(1,1)=32;
	CFaceAlignment fa;
	aliImage = fa.Alignment(oriImage, src_points, dst_points);
	IplImage img=aliImage;    //把mat格式转为IplImage，以便在控件中显示图片
	DrawPicToHDC(img, IDC_S_ALI_IMG); 
}

//光照处理
void CStep::OnIllumination()
{
	if (!aliImage.data)   // 判断是否成功载入图片
	{
		MessageBox("请先进行人脸对齐！", "错误提示！");
		return;
	}

	cv::Mat aliImage_gray;
	//灰度化
	cv::cvtColor(aliImage,aliImage_gray,cv::COLOR_RGB2GRAY);
	//cv::imshow("1",aliImage_gray);
	//小波光照处理
	Retina_Model rw;
	illImage = rw.Retina_Model_Normalization(aliImage_gray);
	normalize(illImage,illImage,0,1,cv::NORM_MINMAX);
	//cv::imshow("2",illImage);
	IplImage img=illImage;    //把mat格式转为IplImage，以便在控件中显示图片
	DrawPicToHDC(img, IDC_S_ILL_IMG2); 
}

//识别
void CStep::OnFaceRec()
{
	if (!illImage.data)   // 判断是否成功载入图片
	{
		MessageBox("请先进行光照处理！", "错误提示！");
		return;
	}
	/*cv::imshow("",illImage);
	cv::waitKey();*/
	cv::Mat_<double> train_Img,assist_Img,test_Img; //训练集、辅助集和测试集矩阵
	std::string face_Path;   //人脸图片的路径
	cv::Mat_<double> img,imgData;

	Retina_Model rw;

	for(int i=1;i<=11;i++)
	{
		std::ostringstream oss;
		oss<<i;
		face_Path="Demo2//"+oss.str()+".jpg";
		cv::Mat_<double> src=cv::imread(face_Path,0);
		//小波光照处理
		img = rw.Retina_Model_Normalization(src);
		normalize(img,img,0,1,cv::NORM_MINMAX);
		/*cv::imshow("", img);
		cv::waitKey();*/
		if(img.data)
		{
			cv::resize(img,imgData,cv::Size(20,20));
			imgData=imgData.t();
			cv::Mat_<double> colImgs=imgData.reshape(imgData.rows*imgData.cols);
			train_Img.push_back(colImgs);
		}
	}
	train_Img=train_Img.t();  //转置后得到最终的训练集矩阵（图片以列存放）
	cv::FileStorage oos(".\\Assist.xml", cv::FileStorage::READ);  
	oos["Assist"] >>assist_Img;     //得到辅助集矩阵
	oos.release();

	cv::Mat test=illImage.clone();  //复制一份人脸图像
	cv::Mat test_resize;
	cv::resize(test, test_resize,cv::Size(20,20));
	cv::Mat gray;
	// 灰度化
	if(test_resize.channels()==3)
		cv::cvtColor(test_resize,gray,CV_BGR2GRAY);
	else
		gray=test_resize;

	cv::Mat_<double> testData=gray;  //把图像数据转为double型，便于后面的人脸识别
	testData=testData.t();
	test_Img=testData.reshape((testData.rows)*(testData.cols));   //得到测试图片的矩阵
	test_Img = test_Img.t();
	//调用人脸识别函数
	FaceRec a;

	int result=a.recognition2(train_Img,assist_Img,test_Img);

	std::ostringstream oss;
	oss<<result;
	face_Path="Demo2//"+oss.str()+".jpg";
	resImage=cv::imread(face_Path);
	IplImage img_show=resImage;    //把mat格式转为IplImage，以便在控件中显示图片
	DrawPicToHDC(img_show, IDC_S_RES_IMG); 
}




void CStep::OnCarDetect()
{
	// TODO:  在此添加命令处理程序代码
	if (!oriImage.data)   // 判断是否成功载入图片
	{
		MessageBox("请先读入原图！", "错误提示！");
		return;
	}

	FaceDetect2 fd;
	fd.draw(oriImage);
	carImage = fd.car_Image;
	IplImage img = carImage;    //把mat格式转为IplImage，以便在控件中显示图片
	DrawPicToHDC(img, IDC_S_CAR_IMG);
}
