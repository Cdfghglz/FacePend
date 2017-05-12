#include "detector.h"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;

Detector::Detector(const int camId, const int DISPLAY_WIDTH, int* displayHeight) : camId_(camId) {

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

cv::Mat Detector::captureFrame() {

	Mat rawFrame;
	cap.read(rawFrame);

	cvtColor(rawFrame, displayFrame, COLOR_BGR2GRAY);
	resize(displayFrame, displayFrame, Size(frameWidth_, frameHeight_), 0, 0, CV_8UC1);
//	std::cout << displayFrame.size() << std::endl;

	equalizeHist(displayFrame, displayFrame);

	return rawFrame;
}

cv::Mat Detector::getFrame() {
	return displayFrame;
}

FaceResult Detector::detectFace() {

	FaceResult faceRes;
	std::vector<Rect> faces;

	face_cascade.detectMultiScale( displayFrame, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
	for ( size_t i = 0; i < faces.size(); i++ )
	{
		faceRes.valid = 1;

		cv::Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
//		ellipse( displayFrame, center, Size( faces[i].width/2, faces[i].height/2 ), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );

		//todo: return all faces with sizes
		if (i == 0){
			faceRes.facePos.x = ((double)center.x/frameWidth_*2 - 1);
			faceRes.facePos.y = ((double)center.y/frameHeight_*2 - 1);

//			some magic numbers, computed by fitting the equation y=c1/x+c2 with face sizes (x) at distances from cam (y)
			float c1 = -231.0f, c2 = 21.0f;
			faceRes.facePos.z = c2 + c1 / (sqrt(( faces[0].height )^2 * ( faces[0].width )^2) );

//			std::cout << faceRes.facePos.x << " and " << faceRes.facePos.y << " and " << faceRes.facePos.z << std::endl;
		}

	}

	return faceRes;
}
