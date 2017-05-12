#include <iostream>
#include <SDL2/SDL.h>
#include "opencv2/core/mat.hpp"

#include "common.h"
#include "pendulum.h"
#include "pendulum.cpp"
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "detector.h"
#include "tracker.h"

static const int DISPLAY_WIDTH = 600;
static int DISPLAY_HEIGHT;
static const int ARM_LENGTH = 2;
static const double INIT_THETA = 0.89;
static const float BG_INIT_Z = -10.0f;

int main(int argc, char **argv) {

	Detector detector(0, DISPLAY_WIDTH, &DISPLAY_HEIGHT);
	FaceTracker tracker;

	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");

	Mesh star("../res/ee.obj");
	Mesh background("../res/plane.obj");
	Shader starShader("../shd/basicShader");
	Shader bgShader("../shd/basicShader");
	Texture texture("../res/bricks.jpg");

#ifdef TRACK_DEBUG
	Mesh centerStar("../res/ee.obj");
	Shader centerStarShader("../shd/trackDebugShader");
	Transform centerStarTransform;
	centerStarTransform.GetRot()->x = 1.57;        // Bring model to the initial orientation
#endif

	Transform starTransform;
	Transform bgTransform;
	Camera camera(glm::vec3(0.0f, 0.0f, 20.0f),
				  70.0f,
				  (float) DISPLAY_WIDTH / (float) DISPLAY_HEIGHT,
				  0.1f,
				  100.0f);

	Pendulum<double> *pendulum = new Pendulum<double>(ARM_LENGTH, 10.0);
	pendulum->reset(INIT_THETA);

//	get background to the initial position and appropriate scale
	Point<double> p = {1.0, 1.0};
	camera.viewToWorld(&p, BG_INIT_Z);
	bgTransform.GetPos()->z = BG_INIT_Z;    // Bring bg plane to initial z distance
	bgTransform.GetScale()->x = (float) p.x;
	bgTransform.GetScale()->y = (float) p.x * DISPLAY_HEIGHT / DISPLAY_WIDTH;

	starTransform.GetRot()->x = 1.57;        // Bring model to the initial orientation

//	other inits
	display.captureCursor();
	float ctr = 0.0f;
	SDL_Event e;
	bool isRunning = true;

	while (isRunning) {

		display.Clear(0.0f, 0.0f, 0.0f, 1.0f);
		Texture capTexture(detector.captureFrame(), GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				isRunning = false;
#ifdef USE_MOUSE
//			manual z control
//			else if (e.type == SDL_KEYDOWN) {
//				starTransform.GetPos()->z += 0.5f;
//				std::cout<< starTransform.GetPos()->z << std::endl;
//			}
//			else if (e.type == SDL_KEYUP) {
//				starTransform.GetPos()->z -= 0.5f;
//				std::cout << starTransform.GetPos()->z << std::endl;
//			}
#endif
			else if (e.type == SDL_KEYUP) {
				Point<double> p = {0, 0};
				pendulum->setAccel(p);
				std::cout << "Accelerations set to 0" << std::endl;
			}
		}

#if USE_MOUSE
		display.captureCursor();

		Point<double> centerPos = display.getCursor();
		Point<double> accel = display.getCursorAccel();

		cv::Point3d faceCenter = {-centerPos.x, -centerPos.y, 0};

		FaceResult faceResult;

		ctr += 0.1f;
		std::cout << ctr << std::endl;
		if (ctr >= 10) {ctr = 0.0f; faceResult.valid = 0;}
		else if (ctr >= 5) {faceResult.valid = 1;};

		faceResult.facePos = {-centerPos.x, -centerPos.y, 0};
//		faceResult.facePos = {-centerPos.x, -centerPos.y, 8*sin(ctr)};

#else
		FaceResult faceResult = detector.detectFace();
		// todo: implement generic acceleration calculator in tracker.cpp, from diplay.cpp
		Point<double> accel(0, 0);
#endif

		tracker.update(faceResult);

		faceResult.facePos = camera.viewToWorld(tracker.getPosition());
		pendulum->setCenter(faceResult.facePos.x, faceResult.facePos.y, accel); // todo: implement the acceleration dependence on depth
		starTransform.GetPos()->z = (float) faceResult.facePos.z;

		pendulum->step();
		Point<double> pos = pendulum->getEndPosition();

		starTransform.GetPos()->x = (float) pos.x;
		starTransform.GetPos()->y = (float) pos.y;

//		background
		bgShader.Bind();
		capTexture.Bind();
		starShader.Update(bgTransform, camera);
		background.Draw();

//		pendulum
		starShader.Bind();
		texture.Bind();
		starShader.Update(starTransform, camera);
		star.Draw();

#ifdef TRACK_DEBUG
		centerStarTransform.GetPos()->x = (float) faceResult.facePos.x;
		centerStarTransform.GetPos()->y = (float) faceResult.facePos.y;
		centerStarTransform.GetPos()->z = (float) faceResult.facePos.z;
		centerStarShader.Bind();
		texture.Bind();
		centerStarShader.Update(centerStarTransform, camera, (float) faceResult.valid);
		centerStar.Draw();
#endif

		display.SwapBuffers();
		SDL_Delay(1);

	}

	return 0;
}
