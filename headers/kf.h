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
//	float dt_ = 0.1f;
//	float processCov_ = 1e-4;
//	float measCov_ = 10.0f;
//	float errCov_ = 0.1f;
	float dt_ = 1.0f/10.0f;
	float processCov_ = 1/1000.0f;
	float measCov_ = 1.0f/1000.0f;
	float errCov_ = 1000.0f/1.0f;

public:
//	KF(int stateSize) : KF_(2 * stateSize, stateSize, 0) { };
	KF(int stateSize) : KF_(4, 2, 0) { };

	void initialize(Point<float> mousePos) {

		KF_.transitionMatrix = cv::Mat_<float>(4, 4) <<
				1, 0, dt_, 0,
				0, 1, 0, dt_,
				0, 0, 1, 0,
				0, 0, 0, 1;

		KF_.statePre.at<float>(0) = mousePos.x;
		KF_.statePre.at<float>(1) = mousePos.y;
		KF_.statePre.at<float>(2) = 0;
		KF_.statePre.at<float>(3) = 0;

		KF_.measurementMatrix = cv::Mat_<float>(2, 4) <<
				1, 0, 0, 0,
				0, 1, 0, 0;

		KF_.processNoiseCov= cv::Mat_<float>(4, 4) <<
		0.00025, 0.0,  0.0005,  0.0,
		0.0,  0.00025,  0.0,  0.0005,
		0.0005,  0.0,  0.001,  0.0,
		0.0,  0.0005,  0.0,  0.001;

		KF_.measurementNoiseCov = cv::Mat_<float>(2, 2) <<
			measCov_, 0,
			0, measCov_;

		setIdentity(KF_.errorCovPost, cv::Scalar::all(errCov_));
	};

	void initialize() {

		KF_.transitionMatrix = cv::Mat_<float>(6, 6) <<
			    1, 0, 0, 1, 0, 0,
				0, 1, 0, 0, 1, 0,
				0, 0, 1, 0, 0, 1,
				0, 0, 0, 1, 0, 0,
				0, 0, 0, 0, 1, 0,
				0, 0, 0, 0, 0, 1;

		cv::Mat_<float> measurement(3, 1);
		measurement.setTo(cv::Scalar(0));

//		KF_.statePre.at<float>(0) = (float) initState.x;
//		KF_.statePre.at<float>(1) = (float) initState.y;
//		KF_.statePre.at<float>(2) = (float) initState.z;

		KF_.statePre.at<float>(0) = 0.2f;
		KF_.statePre.at<float>(1) = 0.2f;
		KF_.statePre.at<float>(2) = 0.2f;
		KF_.statePre.at<float>(3) = 0;
		KF_.statePre.at<float>(4) = 0;
		KF_.statePre.at<float>(5) = 0;
		setIdentity(KF_.measurementMatrix);
		setIdentity(KF_.processNoiseCov, cv::Scalar::all(1e-4));
		setIdentity(KF_.measurementNoiseCov, cv::Scalar::all(10));
		setIdentity(KF_.errorCovPost, cv::Scalar::all(.1));
	};

	void filter(Point<double>& measurement) {
		cv::Mat prediction = KF_.predict();

		cv::Mat_<float> matMeasurement(2, 1);
		matMeasurement(0) = (float)measurement.x;
		matMeasurement(1) = (float)measurement.y;
		cv::Mat estimated = KF_.correct(matMeasurement);

		std::cout << "--" << std::endl;
		std::cout << measurement.x << " | " << measurement.y << std::endl;
		std::cout << matMeasurement << std::endl;
//		std::cout << estimated.size << " | " << estimated.at<float>(0) << " | " << estimated.at<float>(3) << std::endl;
		std::cout << estimated << std::endl;

		measurement.x = (double)estimated.at<float>(0);
		measurement.y = (double)estimated.at<float>(1);

	};

	void filter(cv::Point3d& measurement) {
		cv::Mat prediction = KF_.predict();

		std::cout << measurement.x << std::endl;

		cv::Mat_<float> matMeasurement(3, 1);
		matMeasurement(0) = (float)measurement.x;
		matMeasurement(1) = (float)measurement.y;
		matMeasurement(2) = (float)measurement.z;

		cv::Mat estimated = KF_.correct(matMeasurement);

		measurement.x = estimated.at<double>(0);
		measurement.y = estimated.at<double>(1);
		measurement.z = estimated.at<double>(2);

		std::cout << measurement.x << std::endl;
	};
};

#endif //GL_PENDULUM_KF_H
