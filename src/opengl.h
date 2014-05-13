#ifndef OPENGL_H
#define OPENGL_H

#ifdef __APPLE__ // Defined on Mac OS X

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#elif __linux__ // Defined on Linux
#include <GL/gl.h>
#include <GL/glut.h>

#elif _WIN32
#error OS not supported
#else
#error OS not supported
#endif

#endif // OPENGL_H