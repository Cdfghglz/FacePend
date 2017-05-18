# FacePend
Small AR application with interactive pendulum tracking a face in webcam stream using OpenCV and OpenGL.

## Requirements
- OpenCV 3.2+
- OpenGL 3+
- SDL2
- Boost 1.6+

## Instructions
Use `cmake` and `make` to build, run `./gl_pendulum`.

In CMakeLists.txt define:
- USE_MOUSE to use mouse pointer for control instead
- TRACK_DEBUG to indicate recognition state and tracked position
