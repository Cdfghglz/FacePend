#include <iostream>
#include <SDL2/SDL.h>

#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "common.h"
#include "pendulum.h"
#include "pendulum.cpp"
#include "transform.h"
#include "tracker.h"
#include "camera.h"

static const int DISPLAY_WIDTH = 400;
static int DISPLAY_HEIGHT;
static const int ARM_LENGTH = 2;
static const double INIT_THETA = 0.89;

int main(int argc, char** argv)
{

	Tracker tracker(0, DISPLAY_WIDTH, &DISPLAY_HEIGHT);

	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");

	Mesh star("../res/ee.obj");
	Mesh background("../res/plane.obj");
	Shader shader("../shd/basicShader");
	Shader bgShader("../shd/basicShader");
	Texture texture("../res/bricks.jpg");

	Transform transform;
	Transform trs;
	Camera camera(glm::vec3(0.0f, 0.0f, -20.0f), 70.0f, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT, 0.1f, 100.0f);

	SDL_Event e;
	bool isRunning = true;

	Pendulum<double>* pendulum = new Pendulum<double>(ARM_LENGTH, 10);
	pendulum->reset(INIT_THETA);

//	get to initial position
	trs.GetRot()->x = 3.14;
	trs.GetPos()->z = -15;
	transform.GetRot()->x = 1;
	while(isRunning)
	{
		while(SDL_PollEvent(&e))
		{
			if(e.type == SDL_QUIT)
				isRunning = false;
		}

		display.Clear(0.0f, 0.0f, 0.0f, 1.0f);

		Texture capTexture(tracker.captureFrame(), GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

		Point<double> centerPos = tracker.detectFace();
//		Point<double> centerPos = display.getCursor();

		camera.viewToWorld(&centerPos);
		pendulum->setCenter(centerPos);

		pendulum->increment(0);
		Point<double> pos = pendulum->getPosition();

		transform.GetPos()->x = pos.x;
		transform.GetPos()->y = - pos.y;

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
