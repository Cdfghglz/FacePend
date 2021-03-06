//
// Created by cdfghglz on 11.5.2017.
//

#ifndef GL_PENDULUM_TRACKER_H
#define GL_PENDULUM_TRACKER_H

// todo: tune to be faster!
static const double dt = 1.0/10.;
static const double measCov = 1.0/600.0;
static const double f1 = 0.0100;				// tuning parameter
static const double f2 = 1.0;					// tuning parameter
static const double procCov1 = f1 * 0.00025;
static const double procCov2 = f1 * f2 * f2 * 0.0005;
static const double procCov3 = f1 * f2 * f2 * f2 * 0.001;
static const double returnSmoothness_ = 2000.0; // magic number, the higher the smoother the return behavior

#include "dlib/filtering.h"
#include "dlib/matrix.h"
#include "common.h"
#include "opencv2/core/mat.hpp"
#include "detector.h"

class Tracker {
private:
	unsigned int idleCt_= 25;

	dlib::kalman_filter<6, 3> trackingFilter_;
	cv::Point3d currentPos_;

	void initTrackingFilter();

public:
	unsigned int missedCtr_ = 0;

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
