
#include "tracker.h"

Tracker::Tracker() {
	initTrackingFilter();
	initVoidFilter();
}

void Tracker::initTrackingFilter() {

	using namespace dlib;

	matrix<double, 6, 6> transitionMat;
	transitionMat =
			1, 0, 0, dt, 0, 0,
			0, 1, 0, 0, dt, 0,
			0, 0, 1, 0, 0, dt,
			0, 0, 0, 1, 0, 0,
			0, 0, 0, 0, 1, 0,
			0, 0, 0, 0, 0, 1;

	matrix<double, 3, 6> measurementMat;
	measurementMat =
			1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0,
			0, 0, 1, 0, 0, 0;

	matrix<double, 6, 6> processNoiseCov;
	processNoiseCov =
			procCov1, 0.0, 0.0, procCov2, 0.0, 0.0,
			0.0, procCov1, 0.0, 0.0, procCov2, 0.0,
			0.0, 0.0, procCov1, 0.0, 0.0, procCov2,
			procCov2, 0.0, 0.0, procCov3, 0.0, 0.0,
			0.0, procCov2, 0.0, 0.0, procCov3, 0.0,
			0.0, 0.0, procCov2, 0.0, 0.0, procCov3;

	matrix<double, 3, 3> measurementNoiseCov;
	measurementNoiseCov =
			measCov, 0, 0,
			0, measCov, 0,
			0, 0, measCov;

	trackingFilter_.set_measurement_noise(measurementNoiseCov);
	trackingFilter_.set_process_noise(processNoiseCov);
	trackingFilter_.set_observation_model(measurementMat);
	trackingFilter_.set_transition_model(transitionMat);

}

void Tracker::initVoidFilter() {

}

void Tracker::updateFrame(cv::Point3d position){

	dlib::vector<double, 3> dlibPos;

	dlibPos.x() = position.x;
	dlibPos.y() = position.y;
	dlibPos.z() = position.z;
	trackingFilter_.update(dlibPos);

	dlib::matrix<double, 6, 1> ret;
	ret = trackingFilter_.get_predicted_next_state();

	currentPos_.x = ret(0);
	currentPos_.y = ret(1);
	currentPos_.z = ret(2);

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