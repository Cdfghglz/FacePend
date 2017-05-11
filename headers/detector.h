
#ifndef GL_PENDULUM_DETECTOR_H
#define GL_PENDULUM_DETECTOR_H

#include "opencv2/objdetect.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/highgui.hpp"
#include "headers/common.h"

struct FaceResult {
	bool valid;
	cv::Point3d facePos;
};

class Detector{

public:
	Detector(const int camId, const int displayWidth, int* displayHeight);

	cv::Mat captureFrame();
	cv::Point3d detectFace();
	cv::Mat getFrame();

private:
	const int camId_;
	int frameWidth_;
	int frameHeight_;

	cv::CascadeClassifier face_cascade;
	cv::VideoCapture cap;
	cv::Mat displayFrame;

};

#endif //GL_PENDULUM_DETECTOR_H
