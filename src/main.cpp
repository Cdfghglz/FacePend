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
//static const float REL_FACE_SCALE = 180.002f;

int main(int argc, char **argv) {

	Detector detector(0, DISPLAY_WIDTH, &DISPLAY_HEIGHT);
	FaceTracker tracker;

	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");

	Mesh star("../res/ee.obj");
	Mesh background("../res/plane.obj");
	Shader shader("../shd/basicShader");
	Shader bgShader("../shd/basicShader");
	Texture texture("../res/bricks.jpg");

	Transform transform;
	Transform trs;
	Camera camera(glm::vec3(0.0f, 0.0f, 20.0f), 70.0f, (float) DISPLAY_WIDTH / (float) DISPLAY_HEIGHT, 0.1f, 100.0f);

	Pendulum<double> *pendulum = new Pendulum<double>(ARM_LENGTH, 10.0);
	pendulum->reset(INIT_THETA);

//	get background to the initial position and appropriate scale
	Point<double> p = {1.0, 1.0};
	camera.viewToWorld(&p, BG_INIT_Z);
	trs.GetPos()->z = BG_INIT_Z;    // Bring bg plane to initial z distance
	trs.GetScale()->x = (float) p.x;
	trs.GetScale()->y = (float) p.x * DISPLAY_HEIGHT / DISPLAY_WIDTH;

	transform.GetRot()->x = 1.57;        // Bring model to the initial orientation

//	other inits
	display.captureCursor();
	float ctr = 0.0f;
	SDL_Event e;
	bool isRunning = true;

	while (isRunning) {
		display.Clear(0.0f, 0.0f, 0.0f, 1.0f);

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				isRunning = false;
//			else if (e.type == SDL_KEYDOWN) {
//				transform.GetPos()->z += 0.5f;
//				std::cout<< transform.GetPos()->z << std::endl;
//			}
//			else if (e.type == SDL_KEYUP) {
//				transform.GetPos()->z -= 0.5f;
//				std::cout << transform.GetPos()->z << std::endl;
//			}
			else if (e.type == SDL_KEYUP) {
				Point<double> p = {0, 0};
				pendulum->setAccel(p);
				std::cout << "Accelerations set to 0" << std::endl;
			}
		}


//		camera.viewToWorld(&accel);
//		std::cout << accel << std::endl;

		Texture capTexture(detector.captureFrame(), GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

#if USE_MOUSE
		display.captureCursor();

		Point<double> centerPos = display.getCursor();
		Point<double> accel = display.getCursorAccel();

		cv::Point3d faceCenter = {-centerPos.x, -centerPos.y, 0};

		FaceResult faceResult;

		ctr += 0.1f;
		if (ctr <= 10) faceResult.valid = 1;
		else if (ctr >= 20) ctr = 0.0f;
		faceResult.facePos = {-centerPos.x, -centerPos.y, 8*sin(ctr)};

#else
		cv::Point3d faceCenter = detector.detectFace();
#endif

		tracker.update(faceResult);

		faceResult.facePos = camera.viewToWorld(tracker.getPosition());
		pendulum->setCenter(faceResult.facePos.x, faceResult.facePos.y, accel); // todo: implement the acceleration dependence on depth
		transform.GetPos()->z = (float) faceCenter.z;

		pendulum->step();
		Point<double> pos = pendulum->getEndPosition();

		transform.GetPos()->x = (float) pos.x;
		transform.GetPos()->y = (float) pos.y;

//		background
		bgShader.Bind();
		capTexture.Bind();
		shader.Update(trs, camera);
		background.Draw();

//		pendulum
		shader.Bind();
		texture.Bind();
		shader.Update(transform, camera);
		star.Draw();

		display.SwapBuffers();
		SDL_Delay(1);

	}

	return 0;
}
