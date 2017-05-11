//
// Created by cdfghglz on 11.5.2017.
//

#ifndef GL_PENDULUM_TRACKER_H
#define GL_PENDULUM_TRACKER_H

#include "dlib/filtering.h"
#include "dlib/matrix.h"
#include "common.h"
#include "opencv2/core/mat.hpp"
#include "detector.h"

class Tracker {
private:
	unsigned int bufLen_ = 10;
	unsigned int missedCtr_;

//	CBuf posBuf_(bufLen_);
	dlib::kalman_filter<4, 2> trackingFilter_;
	dlib::kalman_filter<4, 2> voidFilter_;
	cv::Point3d currentPos_;

	void initTrackingFilter();
	void initVoidFilter();

public:
	Tracker();
	void updateFrame(cv::Point3d position);
	void missedFrame();
	cv::Point3d getPosition();

};

// simple wrapper for Tracker with Detector's FaceResult
class FaceTracker : public Tracker {
public:
	void update(FaceResult);
};

#endif //GL_PENDULUM_TRACKER_H
