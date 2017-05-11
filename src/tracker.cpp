
#include "tracker.h"

Tracker::Tracker() {
	initTrackingFilter();
	initVoidFilter();
}

void Tracker::initTrackingFilter() {

	using namespace dlib;

	float dt = 1.0f/10.f;
	float measCov = 1.0f/1000.0f;

	matrix<double, 4, 4> transitionMat;
	transitionMat =
			1, 0, dt, 0,
			0, 1, 0, dt,
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
			measCov, 0,
			0, measCov;

	trackingFilter_.set_measurement_noise(measurementNoiseCov);
	trackingFilter_.set_process_noise(processNoiseCov);
	trackingFilter_.set_observation_model(measurementMat);
	trackingFilter_.set_transition_model(transitionMat);

}

void Tracker::initVoidFilter() {

}

void Tracker::updateFrame(cv::Point3d position){

	dlib::vector<double, 2> dlibPos;

	dlibPos.x() = position.x;
	dlibPos.y() = position.y;
	trackingFilter_.update(dlibPos);

	dlib::matrix<double, 4, 1> ret;
	ret = trackingFilter_.get_predicted_next_state();

	currentPos_.x = ret(0);
	currentPos_.y = ret(1);

};


cv::Point3d Tracker::getPosition() {
	return currentPos_;
}

void Tracker::missedFrame() {
	
}

// --------------------- FaceTracker
void FaceTracker::update(FaceResult result) {
	if (result.valid) updateFrame(result.facePos);
	else missedFrame();
};