#include "model.h"
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>

Model model;


void initialize()
{
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, 640, 480);
	glLoadIdentity();
	gluPerspective(45, 640.f / 480.f, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glEnable(GL_DEPTH_TEST);

	GLfloat insideLampPos[] = { 2.47, -4.07, 6.2, 0.0 };
	GLfloat insideLampCol[] = { 1.0, 0.667, 0.362, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, insideLampPos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, insideLampCol);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

//	glShadeModel(GL_SMOOTH);

	// Enable transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Enable textures
	glEnable(GL_TEXTURE_2D);
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 2, 10,
			  0, 2.8, 0,
			  0, 1, 0);

	model.draw();
	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Obj");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	initialize();
	try {
		model.load("../../res/starter_file.obj");
	}
	catch (const std::exception& e) {
		fprintf(stderr, "error: %s\n", e.what());
		return 1;
	}

	glutMainLoop();

	return 0;
}

