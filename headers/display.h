#ifndef DISPLAY_INCLUDED_H
#define DISPLAY_INCLUDED_H

#include <string>
#include <boost/circular_buffer.hpp>
#include <queue>
#include <SDL2/SDL.h>
#include "common.h"

class Display
{
public:
	Display(int width, int height, const std::string& title);

	void Clear(float r, float g, float b, float a);
	void SwapBuffers();

	virtual ~Display();
	Point<double> getCursor();
	void captureCursor();
	Point<double> getCursorAccel();

protected:
private:
	void operator=(const Display& display) {}
	Display(const Display& display) {}

	int width_, height_;
//	CircBuf<Point<double> > mousePosQ;
	const unsigned long int mouseBufLen = 3;
	CBuf<Point<double> > mousePosQ;

	SDL_Window* m_window;
	SDL_GLContext m_glContext;
};

#endif
