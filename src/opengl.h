#ifndef OPENGL_H
#define OPENGL_H

#define GLEW_STATIC
#include <GL/glew.h>

#ifdef __APPLE__ // Defined on Mac OS X

#include <GLUT/glut.h>
// OpenAL
#include <OpenAL/al.h>
#include <ALUT/alut.h>

#elif __linux__ // Defined on Linux

#include <GL/glut.h>
// OpenAL
#include <AL/al.h>
#include <AL/alut.h>

#elif _WIN32

#error OS not supported

#else

#error OS not supported

#endif

#endif // OPENGL_H
