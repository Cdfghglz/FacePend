#include "tracker.h"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;

Tracker::Tracker(const int camId, const int DISPLAY_WIDTH, int* displayHeight, const float REL_FACE_SCALE) : camId_(camId), relFaceScale_(REL_FACE_SCALE) {

	if (!face_cascade.load("../res/haarcascade_frontalface_alt.xml"))
		std::cout << "Face cascade could not be loaded!";

	cap.open(camId_);
	if (!cap.isOpened())
		std::cout << "Could not open video capture!";

	cv::Mat testFrame;
	cap.read(testFrame);
	*displayHeight = DISPLAY_WIDTH * testFrame.rows/testFrame.cols;

	cap.set(CV_CAP_PROP_FRAME_HEIGHT, *displayHeight);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, DISPLAY_WIDTH);

	frameHeight_ = *displayHeight;
	frameWidth_ = DISPLAY_WIDTH;

}

cv::Mat Tracker::captureFrame() {

	Mat rawFrame;
	cap.read(rawFrame);

	cvtColor(rawFrame, displayFrame, COLOR_BGR2GRAY);
	resize(displayFrame, displayFrame, Size(frameWidth_, frameHeight_), 0, 0, CV_8UC1);
//	std::cout << displayFrame.size() << std::endl;

	equalizeHist(displayFrame, displayFrame);

	return rawFrame;
}

cv::Mat Tracker::getFrame() {
	return displayFrame;
}
cv::Point3d Tracker::detectFace() {

	cv::Point3d faceCenter;
	std::vector<Rect> faces;

	face_cascade.detectMultiScale( displayFrame, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
	for ( size_t i = 0; i < faces.size(); i++ )
	{
		cv::Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
		ellipse( displayFrame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

		//todo: return all faces with sizes
		if (i == 0){
			faceCenter.x = -((double)center.x/frameWidth_*2 - 1);
			faceCenter.y = -((double)center.y/frameHeight_*2 - 1);

			faceCenter.z = sqrt(( faces[0].height )^2 * ( faces[0].width )^2) * relFaceScale_;
//			std::cout << faceCenter.x << " and " << faceCenter.y << std::endl;
		}

	}

	return faceCenter;
}
