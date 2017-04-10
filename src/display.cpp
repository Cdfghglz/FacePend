#include "display.h"
#include <GL/glew.h>
#include <iostream>
#include <glfw3.h>

Display::Display(int width, int height, const std::string& title) : width_(width), height_(height), mousePosQ(mouseBufLen)
{
	SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	m_glContext = SDL_GL_CreateContext(m_window);

	GLenum res = glewInit();
    if(res != GLEW_OK)
    {
		std::cerr << "Glew failed to initialize!" << std::endl;
    }

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

Display::~Display()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Display::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::SwapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}

Point<double> Display::getCursor() {

	// Get mouse position
	Point<double> p;
	int x, y;
	SDL_GetMouseState(&x, &y);

	p.x = ((double)x/width_*2 - 1);
	p.y = -((double)y/height_*2 - 1);

	return p;

}

void Display::captureCursor() {
	mousePosQ.push(getCursor());
}

Point<double> Display::getCursorAccel() {

	Point<double> accel;
	accel.x = (mousePosQ[2].x - mousePosQ[1].x) - (mousePosQ[1].x - mousePosQ[0].x);
	accel.y = (mousePosQ[2].y - mousePosQ[1].y) - (mousePosQ[1].y - mousePosQ[0].y);

	return accel;
};
