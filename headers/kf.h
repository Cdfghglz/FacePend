//
// Created by cdfghglz on 30.4.2017.
//
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"
#include "common.h"

#ifndef GL_PENDULUM_KF_H
#define GL_PENDULUM_KF_H

class KF {
private:
	cv::KalmanFilter KF_;
public:
	KF() : KF_(4, 2, 0) {};
	void initialize(Point<float> mousePos) {

		KF_.transitionMatrix = cv::Mat_<float>(4, 4) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1;
		cv::Mat_<float> measurement(2,1); measurement.setTo(cv::Scalar(0));

		KF_.statePre.at<float>(0) = mousePos.x;
		KF_.statePre.at<float>(1) = mousePos.y;
		KF_.statePre.at<float>(2) = 0;
		KF_.statePre.at<float>(3) = 0;
		setIdentity(KF_.measurementMatrix);
		setIdentity(KF_.processNoiseCov, cv::Scalar::all(1e-4));
		setIdentity(KF_.measurementNoiseCov, cv::Scalar::all(10));
		setIdentity(KF_.errorCovPost, cv::Scalar::all(.1));
	};

	Point<float> filter(Point<float> measurement) {
		cv::Mat prediction = KF_.predict();
		Point<float> predictPt(prediction.at<float>(0),prediction.at<float>(1));

		cv::Mat_<float> matMeasurement(2,1);
		matMeasurement(0) = measurement.x;
		matMeasurement(1) = measurement.y;
		cv::Mat estimated = KF_.correct(matMeasurement);

		Point<float> res(estimated.at<float>(0), estimated.at<float>(1));
		return res;
	};

};

#endif //GL_PENDULUM_KF_H
