//
// Created by cdfghglz on 30.4.2017.
//

#ifndef GL_PENDULUM_KF_H
#define GL_PENDULUM_KF_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"
#include "dlib/filtering.h"
#include "dlib/matrix.h"
#include "common.h"

using namespace dlib;

template<unsigned int states, unsigned int observations>
class KF {
private:
	kalman_filter<states, observations> kf_;
	float dt_ = 1.0f / 10.0f;
	float processCov_ = 1 / 1000.0f;
	float measCov_ = 1.0f / 1000.0f;
	float errCov_ = 1000.0f / 1.0f;

public:
	void initialize() {
		matrix<double, 4, 4> transitionMat;
		transitionMat =
				1, 0, dt_, 0,
				0, 1, 0, dt_,
				0, 0, 1, 0,
				0, 0, 0, 1;


		matrix<double, 2, 4> measurementMat;
		measurementMat =
				1, 0, 0, 0,
				0, 1, 0, 0;

		matrix<double, 4, 4> processNoiseCov;
		processNoiseCov =
				0.00025, 0.0,  0.0005,  0.0,
				0.0,  0.00025,  0.0,  0.0005,
				0.0005,  0.0,  0.001,  0.0,
				0.0,  0.0005,  0.0,  0.001;

		matrix<double, 2, 2> measurementNoiseCov;
		measurementNoiseCov =
				measCov_, 0,
				0, measCov_;

		kf_.set_measurement_noise(measurementNoiseCov);
		kf_.set_process_noise(processNoiseCov);
		kf_.set_observation_model(measurementMat);
		kf_.set_transition_model(transitionMat);

//		double kf_error = test_filter(kf, 300);
	}

	void filter(double x, double y) {
		dlib::vector<double,2> pp;
		pp.x() = x;
		pp.y() = y;
		kf_.update(pp);

		matrix<double,states,1> ret;
		ret = kf_.get_predicted_next_state();

	}


	void filter(Point<double>& measurement) {

		dlib::vector<double,2> pp;
		pp.x() = measurement.x;
		pp.y() = measurement.y;

		std::cout << measurement.x << std::endl;
		std::cout << measurement << std::endl;

		kf_.update(pp);

		matrix<double,states,1> ret;
		ret = kf_.get_predicted_next_state();

		measurement.x = ret(0);
		measurement.y = ret(1);

		std::cout << measurement << std::endl;
	};

	void filter(cv::Point3d& measurement) {

		dlib::vector<double,2> pp;
		pp.x() = measurement.x;
		pp.y() = measurement.y;

		std::cout << measurement.x << std::endl;

		kf_.update(pp);

		matrix<double,states,1> ret;
		ret = kf_.get_predicted_next_state();

		measurement.x = ret(0);
		measurement.y = ret(1);

		std::cout << measurement.x << std::endl;
	};
};

#endif //GL_PENDULUM_KF_H
