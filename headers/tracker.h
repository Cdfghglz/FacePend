//
// Created by cdfghglz on 26.3.2017.
//

#ifndef GL_PENDULUM_TRACKER_H
#define GL_PENDULUM_TRACKER_H

#include "opencv2/objdetect.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/highgui.hpp"
#include "headers/common.h"

class Tracker {

public:
	Tracker(const int camId, const int displayWidth, int* displayHeight);

	cv::Mat captureFrame();
	::Point<double> detectFace();
	cv::Mat getFrame();

private:
	const int camId_;
//	todo: naming convention!
	int frameWidth_;
	int frameHeight_;

	cv::CascadeClassifier face_cascade;
	cv::VideoCapture cap;
	cv::Mat displayFrame;

};

#endif //GL_PENDULUM_TRACKER_H
