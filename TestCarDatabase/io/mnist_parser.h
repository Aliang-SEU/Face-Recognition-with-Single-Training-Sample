/*
    Copyright (c) 2013, Taiga Nomi
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY 
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once
#include "util/util.h"
#include <fstream>
#include <cstdint>

#include <cv.h>
#include <highgui.h> 
#include <cstring>

namespace tiny_cnn {

inline std::string * readTxt(const std::string& fileName, int size = 1)
{
	std::ifstream infile;
	infile.open(fileName);  //  打开文件夹
	std::string temp;
	std::string * imageInfo = new std::string[size];  //定义数组存放每张图片的所有有用信息
	int count = 0;
	while (getline(infile, temp))  //
	{
		imageInfo[count] = temp;
		count++;
		//
	}

	return imageInfo;
	delete[] imageInfo;
}

inline void loadImage(const std::string& fileName, std::string& imageInfo, vec_t& imageGray, vec_t& label, vec_t& faceArea, std::string& imgName)
{
	//
	const int featurePointNum = 10;

	std::string  imageName;  // 定义数组存放训练图片名称
	std::string facePoint[14];  // ****定义全局变量存放每张图片的大小和特征点  
	std::ifstream infile;
	infile.open(fileName);  //  打开文件夹
	int count = 0;

	int ii = 0;   //统计第一个空格前有多少字符
	for (unsigned int i = 0; i < imageInfo.length(); i++)   //储存图片名//获取输入的训练图片的名称，大小和特征点的精确位置
	{
		if (imageInfo[i] != ' ')
		{
			ii++;
		}
		else
		{
			break;
		}
	}
	imageName = imageInfo.substr(0, ii);
	imgName = imageName;

	int n = 0;   // 定义数组列
	for (unsigned int j = ii + 1; j < imageInfo.length(); j++)   // 储存图片大小和特征点位置
	{
		if (imageInfo[j] != ' ')
		{
			continue;
		}
		facePoint[n] = imageInfo.substr(ii + 1, j - ii - 1);
		ii = j;
		n++;
		//break;
	}
	//
	facePoint[13] = imageInfo.substr(ii + 1, imageInfo.length() - ii - 1);
	//
	faceArea.resize(4);  //  储存图片中人脸的位置
	faceArea[0] = atof(facePoint[0].c_str());
	faceArea[1] = atof(facePoint[1].c_str());
	faceArea[2] = atof(facePoint[2].c_str());
	faceArea[3] = atof(facePoint[3].c_str());

	float_t faceWidth = (atof(facePoint[1].c_str()) - atof(facePoint[0].c_str()));   //  人脸图片的宽度（横坐标）
	float_t faceHeight = (atof(facePoint[3].c_str()) - atof(facePoint[2].c_str()));  //  人脸图片的高度（纵坐标）
	label.resize(featurePointNum); // 给vector变量分配空间
	for (int i = 0; i < featurePointNum; i++)
	{
		if (i % 2 == 0)
		{
			label[i] = (atof(facePoint[4 + i].c_str()) - atof(facePoint[0].c_str())) / faceWidth; // 横坐标归一化
		}
		else
		{
			label[i] = (atof(facePoint[4 + i].c_str()) - atof(facePoint[2].c_str())) / faceHeight; // 纵坐标归一化
		}

	}
	//
	count++;

	IplImage * img;
	IplImage * dst;
	//std::string path;  //  存储文件夹路径
	//path = "D:\\vs2013\\cnnEyesDetection\\cnnEyesDetection\\";
	//imageName = path + imageName;  //获得图片的绝对路径

	//cout << imageName << endl;
	const char * image = imageName.c_str();
	img = cvLoadImage(image, 0);  // 读取图像
	//img = imageName
	//img = cvLoadImage("D:\\vs2013\\cnnEyesDetection\\cnnEyesDetection\\Ariel_Sharon\\Ariel_Sharon_0001.jpg", 1);
	if (img == NULL)
	{
		std::cout << "无法显示图片" << std::endl;

	}

	// 将读入的图像按要求裁剪
	cvSetImageROI(img, cvRect(atoi(facePoint[0].c_str()), atoi(facePoint[2].c_str()),
		atoi(facePoint[1].c_str()) - atoi(facePoint[0].c_str()),
		atoi(facePoint[3].c_str()) - atoi(facePoint[2].c_str())));
	//新建一个与需要剪切的图像部分同样大小的新图像
	dst = cvCreateImage(cvSize(atoi(facePoint[1].c_str()) - atoi(facePoint[0].c_str()), atoi(facePoint[3].c_str()) - atoi(facePoint[2].c_str())),
		IPL_DEPTH_8U,
		img->nChannels);
	//将源图像复制到新建的图像中
	cvCopy(img, dst, 0);
	//释放ROI区域
	cvResetImageROI(img);
	//将裁剪后的图像缩放成32 * 32
	CvSize czSize;
	czSize.height = 32;
	czSize.width = 32;
	IplImage * scalimage = cvCreateImage(czSize, dst->depth, dst->nChannels);
	cvResize(dst, scalimage, CV_INTER_AREA);

	//
	int width = scalimage->width;//图片宽度
	int height = scalimage->height;//图片高度
	//vec_t intensity;
	int i = 0;
	imageGray.resize(width * height);
	for (size_t row = 0; row < height; row++)
	{
		uchar* ptr = (uchar*)scalimage->imageData + row * scalimage->width;//获得灰度值数据指针
		for (size_t cols = 0; cols < width; cols++)
		{
			imageGray[i] = -(ptr[cols] / 255.0 * 2.0 - 1.0);
			i++;
		}
	}
	/*/
	for (int i = 0; i < width * height; i++)
	{
	std::cout << imageGray[i] << "  ";
	if (i % 10 == 9)
	{
	std::cout << std::endl;
	}
	}
	std::cin.get();
	//
	cvNamedWindow("Ariel_Sharon_0001", 1);  // 创建图像框
	cvShowImage("Ariel_Sharon_0001", img);  //  显示图像
	cvWaitKey(0);

	cvNamedWindow("操作后图像", 1);  // 创建新的图像框
	cvShowImage("操作后图像", dst);  //  显示裁剪后的图像
	cvWaitKey(0);

	cvNamedWindow("缩放后的图像", 1);  // 创建新的图像框
	cvShowImage("缩放后的图像", scalimage);  // 显示缩放后的图像
	cvWaitKey(0);

	cvDestroyWindow("缩放后的图像");
	cvDestroyWindow("操作后图像");
	cvDestroyWindow("Ariel_Sharon_0001");
	cvReleaseImage(&img);
	cvReleaseImage(&dst);
	cvReleaseImage(&scalimage);
	/*/
}

inline void loadTestImage(const std::string path, vec_t& imageGray, vec_t& faceArea)  // faceArea用来记录图片的大小
{

	//imgName = imageName;  // 储存图片名

	IplImage * img;
	IplImage * dst;

	//const char * image = path.c_str();

//	*img = IplImage(src);
	img = cvLoadImage(path.c_str(),0);  // 读取图像
	//img = imageName
	//img = cvLoadImage("D:\\vs2013\\cnnEyesDetection\\cnnEyesDetection\\Ariel_Sharon\\Ariel_Sharon_0001.jpg", 1);
	if (img == NULL)
	{
		std::cout << "无法显示图片" << std::endl;

	}

	// 将读入的图像按要求裁剪
	//cvSetImageROI(img, cvRect(atoi(facePoint[0].c_str()), atoi(facePoint[2].c_str()),
	//	atoi(facePoint[1].c_str()) - atoi(facePoint[0].c_str()),
	//	atoi(facePoint[3].c_str()) - atoi(facePoint[2].c_str())));
	//新建一个与需要剪切的图像部分同样大小的新图像

	faceArea.resize(2);
	faceArea[0] = img->width * 1.0;
	faceArea[1] = img->height * 1.0;  // 确定哪个存长哪个存宽

	dst = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, img->nChannels);

	//将源图像复制到新建的图像中
	cvCopy(img, dst, 0);
	//释放ROI区域
	cvResetImageROI(img);
	//将裁剪后的图像缩放成32 * 32
	CvSize czSize;
	czSize.height = 32;
	czSize.width = 32;
	IplImage * scalimage = cvCreateImage(czSize, dst->depth, dst->nChannels);
	cvResize(dst, scalimage, CV_INTER_AREA);

	//
	int width = scalimage->width;//图片宽度
	int height = scalimage->height;//图片高度
	//vec_t intensity;
	int i = 0;
	imageGray.resize(width * height);
	for (size_t row = 0; row < height; row++)
	{
		uchar* ptr = (uchar*)scalimage->imageData + row * scalimage->width;//获得灰度值数据指针
		for (size_t cols = 0; cols < width; cols++)
		{
			imageGray[i] = -(ptr[cols] / 255.0 * 2.0 - 1.0);
			i++;
		}
	}
	/*/
	for (int i = 0; i < width * height; i++)
	{
		std::cout << imageGray[i] << "  ";
		if (i % 10 == 9)
		{
			std::cout << std::endl;
		}
	}
	std::cin.get();
	//
	cvNamedWindow("Ariel_Sharon_0001", 1);  // 创建图像框
	cvShowImage("Ariel_Sharon_0001", img);  //  显示图像
	cvWaitKey(0);

	cvNamedWindow("操作后图像", 1);  // 创建新的图像框
	cvShowImage("操作后图像", dst);  //  显示裁剪后的图像
	cvWaitKey(0);

	cvNamedWindow("缩放后的图像", 1);  // 创建新的图像框
	cvShowImage("缩放后的图像", scalimage);  // 显示缩放后的图像
	cvWaitKey(0);

	cvDestroyWindow("缩放后的图像");
	cvDestroyWindow("操作后图像");
	cvDestroyWindow("Ariel_Sharon_0001");
	cvReleaseImage(&img);
	cvReleaseImage(&dst);
	cvReleaseImage(&scalimage);
	/*/
}

inline void getImageAndLabel(const std::string& image_file,
	std::vector<vec_t> *images, std::vector<vec_t> *labels, std::vector<vec_t> *faceAreas, std::vector<std::string> *imgNames)
{
	int size = 0;  //定义变量储存训练图片个数

	std::ifstream infile;
	infile.open(image_file);  //  打开文件夹

	assert(infile.is_open());
	/*/
	if(!infile.is_open())   //  判断文件夹有没有打开成功
	{
		std::cout << "您输入的文件夹可能不存在，请检查文件路径 " << std::endl;
		//std::cin >> fileName;
		//infile.open(image_file);	
		std::cin.get();
		exit(0);
	}
	/*/

	std::string temp;
	while (getline(infile, temp))  //获取文件中图片的总个数
	{
		size++;
	}
	//cout << size << endl;
	infile.close();
	infile.clear();
	//infile.open(image_file);   //重新连接流文件

	std::string * imageInfo = new std::string[size];  //  将readtxt函数的返回值读取出来
	imageInfo = readTxt(image_file, size);

	//
	for (int i = 0; i < size; i++)
	{
		vec_t image;
		vec_t label;
		vec_t faceArea;
		std::string imgName;
		//			std::cout << imageInfo[i] << std::endl;
		loadImage(image_file, imageInfo[i], image, label, faceArea, imgName);
		//			loadImage(imageInfo[i], image_file, image);
		images->push_back(image);
		labels->push_back(label);
		faceAreas->push_back(faceArea);
		imgNames->push_back(imgName);
	}
	//

}

inline void getTestImage(const std::string path,
	std::vector<vec_t> *images, std::vector<vec_t> *faceAreas,  int size = 1)
{
	for (int i = 0; i < size; i++)
	{
		vec_t image;
		vec_t faceArea;
		//std::string imgName;
		//			std::cout << imageInfo[i] << std::endl;
		loadTestImage(path, image, faceArea);

		images->push_back(image);
		faceAreas->push_back(faceArea);
		//imgNames->push_back(imgName);
	}

}

} // namespace tiny_cnn

