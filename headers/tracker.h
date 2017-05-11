//
// Created by cdfghglz on 11.5.2017.
//

#ifndef GL_PENDULUM_TRACKER_H
#define GL_PENDULUM_TRACKER_H

static const double dt = 1.0/10.;
static const double measCov = 1.0/1000.0;
static const double f1 = 0.0100;
static const double f2 = 1.0;
static const double procCov1 = f1 * 0.00025;
static const double procCov2 = f1 * f2 * f2 * 0.0005;
static const double procCov3 = f1 * f2 * f2 * f2 * 0.001;

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
	dlib::kalman_filter<6, 3> trackingFilter_;
	dlib::kalman_filter<6, 3> voidFilter_;
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
