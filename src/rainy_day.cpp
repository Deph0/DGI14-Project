#include "opengl.h"
#include "rainy_day.h"
#include "settings.h"


RainyDay::RainyDay()
{
	addListener(&scene);
}


void RainyDay::initialize()
{
	glutDisplayFunc(RainyDay::display);
	glutReshapeFunc(RainyDay::reshape);
	glutIdleFunc(RainyDay::idle);
	glutKeyboardFunc(RainyDay::keyDown);
	glutSpecialFunc(RainyDay::specialKeyDown);
	// Process mouse button push/release
	glutMouseFunc(RainyDay::mouseButton);
	// Process mouse dragging motion
	glutMotionFunc(RainyDay::mouseMove);

	// -------------------------------------
	// Enable global settings
	// -------------------------------------
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	// Normalize normals
	glEnable(GL_NORMALIZE);

	// Enable transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable textures
	glEnable(GL_TEXTURE_2D);

	fire(GlutListener::Initialize());
}


void RainyDay::reshape(int width, int height)
{
	RainyDay::getInstance()->fire(GlutListener::Reshape(), width, height);
	// Something is not initialized in correct order.
	// A quick fix for that.
	display();
}


void RainyDay::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	RainyDay::getInstance()->fire(GlutListener::Display());

	glutSwapBuffers();
}


void RainyDay::idle()
{
	static int prevTime = glutGet(GLUT_ELAPSED_TIME);
	int dt = glutGet(GLUT_ELAPSED_TIME) - prevTime;

	if (dt < (1000.f / FRAMES_PER_SECOND))
		return;

	RainyDay::getInstance()->fire(GlutListener::Idle(), dt);
	prevTime += dt;
}


void RainyDay::keyDown(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);

	RainyDay::getInstance()->fire(GlutListener::KeyDown(), key, x, y);
}


void RainyDay::specialKeyDown(int key, int x, int y)
{
	RainyDay::getInstance()->fire(GlutListener::SpecialKeyDown(), key, x, y);
}


void RainyDay::mouseButton(int button, int state, int x, int y)
{
	RainyDay::getInstance()->fire(GlutListener::MouseButton(), button, state, x, y);
}


void RainyDay::mouseMove(int x, int y)
{
	RainyDay::getInstance()->fire(GlutListener::MouseMove(), x, y);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("DGI14 Project - Rainy Window");

	glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("ready for OpenGL 2.0\n");
	if (!GLEW_ARB_vertex_shader || !GLEW_ARB_fragment_shader) {
		fprintf(stderr, "error: shaders are not suppotted\n");
		return 1;
	}

	try {
		RainyDay::getInstance()->initialize();
	}
	catch (const std::exception& e) {
		fprintf(stderr, "error: %s\n", e.what());
		return 1;
	}

	glutMainLoop();
	return 0;
}

