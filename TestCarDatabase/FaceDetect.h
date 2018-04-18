#pragma once
#include <opencv.hpp>
class FaceDetect
{
public:
	FaceDetect(void);
	~FaceDetect(void);

	CvRect  car_detection(IplImage*);
	CvRect  detect_and_draw_car(IplImage*, CvHaarClassifierCascade *);
	CvRect face_detection(IplImage*);
	//bool compare(CvRect*, CvRect*);
	CvRect detect_and_draw_face(IplImage*, CvHaarClassifierCascade *);

	void detect_and_draw(cv::Mat img);

	cv::Mat car_Image, face_Image;
	CvRect face_range;
};

