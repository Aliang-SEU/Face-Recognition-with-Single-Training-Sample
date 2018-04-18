#include "StdAfx.h"
#include "FaceDetect.h"
#include "stdafx.h"
#include"vector"
#include"algorithm"

FaceDetect::FaceDetect(void)
{
}


FaceDetect::~FaceDetect(void)
{
}

bool compare2(CvRect*r1, CvRect*r2){
	return (r1->height*r1->width)>(r2->height*r2->width);
}


void FaceDetect::detect_and_draw(cv::Mat img)
{
	IplImage*src = cvCreateImage(cvSize(img.cols, img.rows), 8, 3);
	*src = IplImage(img);
	/*
	CvRect  car_range = car_detection(src);
	//cvRectangle(src,cvPoint(car_range.x,car_range.y),cvPoint(car_range.x+car_range.width,car_range.y+car_range.height),cvScalar(255,0,0),2,8);
	//cvShowImage("src",src);
	IplImage*car = cvCreateImage(cvSize(car_range.width, car_range.height), 8, 3);

	cvSetImageROI(src, car_range);//提取车辆区域
	cvCopy(src, car);
	cvResetImageROI(src);
	car_Image = cv::Mat(car);*/


	CvRect face_range = face_detection(src);
	this->face_range = face_range;
	IplImage*face = cvCreateImage(cvSize(face_range.width, face_range.height), 8, 3);
	cvSetImageROI(src, face_range);//提取人脸区域
	cvCopy(src, face);
	cvResetImageROI(src);

	//cvReleaseImage(&car);
	face_Image = cv::Mat(face);
}

CvRect FaceDetect::car_detection(IplImage * image){

	//cvShowImage("image",image);//用作测试
	//cvWaitKey();//用作测试
	const char * cascade_car = "classifier_car.xml";
	CvHaarClassifierCascade * cascade = 0;
	cvReleaseHaarClassifierCascade(&cascade);//不知道为什麽，照搬别人的解决办法
	cascade = (CvHaarClassifierCascade *)cvLoad(cascade_car, 0, 0, 0);//加载分类器

	CvRect car_range = detect_and_draw_car(image, cascade);
	if (car_range.x)
		return car_range;
	else
		return cvRect(0, 0, 0, 0);
}

CvRect  FaceDetect::detect_and_draw_car(IplImage*image, CvHaarClassifierCascade *cascade)
{
	double scale = 3.0;//检测缩放尺寸
	//cvShowImage("image",image);//用作测试
	//cvWaitKey();//用作测试
	IplImage *gray = cvCreateImage(cvGetSize(image), 8, 1);
	cvCvtColor(image, gray, CV_BGR2GRAY);
	IplImage *small_gray = cvCreateImage(cvSize((int)(gray->width / scale), (int)(gray->height / scale)), 8, 1);
	cvResize(gray, small_gray);
	cvSmooth(small_gray, small_gray, CV_GAUSSIAN, 3);//高斯滤波
	//cvShowImage("small_gray",small_gray);//用作测试
	//cvWaitKey();//用作测试
	CvMemStorage * storage = cvCreateMemStorage(0);
	cvClearMemStorage(storage);
	CvSeq * objects = cvHaarDetectObjects(small_gray, cascade, storage, 1.1, 2, 0, cvSize(30, 30));
	std::vector<CvRect *>cars;
	cars.clear();
	for (int i = 0; i < (objects ? objects->total : 0); i++)
	{
		CvRect *r = (CvRect*)cvGetSeqElem(objects, i);
		cars.push_back(r);
	}
	CvRect car = cvRect(0, 0, 0, 0);
	if (!cars.empty()){
		sort(cars.begin(), cars.end(), compare2);
		car.x = (int)(cars[0]->x*scale);
		car.y = (int)(cars[0]->y*scale);
		car.height = (int)(cars[0]->height*scale);
		car.width = (int)(cars[0]->width*scale);
	}
	cvReleaseImage(&small_gray);
	cvReleaseImage(&gray);
	if (car.x)
		return car;
	else
		return car;
}


CvRect FaceDetect::face_detection(IplImage * image){

	//cvShowImage("image",image);//用作测试
	//cvWaitKey();//用作测试
	const char * cascade_face = "xml4.xml";
	CvHaarClassifierCascade * cascade = 0;
	cvReleaseHaarClassifierCascade(&cascade);//不知道为什麽，照搬别人的解决办法
	cascade = (CvHaarClassifierCascade *)cvLoad(cascade_face, 0, 0, 0);//加载分类器

	CvRect face_range = detect_and_draw_face(image, cascade);
	if (face_range.x)
		return face_range;
	else
		return cvRect(0, 0, 0, 0);
}

CvRect FaceDetect::detect_and_draw_face(IplImage*image, CvHaarClassifierCascade *cascade){
	//cvShowImage("image",image);//用作测试
	//cvWaitKey();//用作测试
	IplImage *gray = cvCreateImage(cvGetSize(image), 8, 1);
	cvCvtColor(image, gray, CV_BGR2GRAY);
	cvSmooth(gray, gray, CV_GAUSSIAN, 3);//高斯滤波
	CvMemStorage * storage = cvCreateMemStorage(0);
	cvClearMemStorage(storage);
	CvSeq * objects = cvHaarDetectObjects(gray, cascade, storage, 1.1, 2, 0, cvSize(30, 30));
	std::vector<CvRect *>faces;
	faces.clear();
	for (int i = 0; i < (objects ? objects->total : 0); i++)
	{
		CvRect *r = (CvRect*)cvGetSeqElem(objects, i);
		faces.push_back(r);
		//cvRectangle(image, cvPoint(r->x, r->y), cvPoint(r->x + r->width, r->y + r->height), cvScalar(255, 0, 0), 2, 8);
	}
	//cvShowImage("image",image);//用作测试
	CvRect face = cvRect(0, 0, 0, 0);
	if (!faces.empty()){
		sort(faces.begin(), faces.end(), compare2);
		face.x = (int)(faces[0]->x);
		face.y = (int)(faces[0]->y);
		face.height = (int)(faces[0]->height);
		face.width = (int)(faces[0]->width);
	}
	cvReleaseImage(&gray);
	if (face.x)
		return face;
	else
		return face;
}