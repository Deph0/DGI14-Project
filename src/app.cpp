#include "app.h"
#include <stdio.h>
#include <GL/glut.h>
#include "dae_model.h"
#include "point_light.h"


Scene App::scene;
glm::ivec2 App::mousePos;


App::App()
{
	DaeModel model;
	std::string fname("starter_file.dae");

	model.load(RESOURCE_PATH + fname, &scene);

	glutDisplayFunc(App::display);
	glutReshapeFunc(App::reshape);
	glutKeyboardFunc(App::keyboard);
	glutSpecialFunc(App::specialInput);
	// Process mouse button push/release
	glutMouseFunc(App::mouseButton);
	// Process mouse dragging motion
	glutMotionFunc(App::mouseMove);

	initialize();

	glutMainLoop();
}


void App::initialize()
{
	// -------------------------------------
	// Enable global settings
	// -------------------------------------

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	// Enable antialiasing (if supported)
	glEnable(GL_MULTISAMPLE);
	glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

	// Normalize normals
	glEnable(GL_NORMALIZE);

	// Enable transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Enable textures
	glEnable(GL_TEXTURE_2D);

	// -------------------------------------
	// Scene spesific
	// -------------------------------------
	Camera* c = &scene.camera;

	// LookAt not present in dae file
	c->lookAt = glm::vec3(0, 0, 3);
	// Blender sets very strange rotation by default
	c->resetRotation();
	// Setting the best view position found with
	// scene.camera.print() in display function
	c->position = glm::vec3(-0.587549, -9.783041, 3.032476);

	// Fix too darkness
	PointLight* lamp = (PointLight*)scene.getByName("InsideLamp");
	if (lamp) {
		lamp->color = lamp->color * 7.f;
	}
	Geometry* plane = (Geometry*)scene.getByName("BkgndPlane");
	if (plane) {
		plane->material->diffuse->color = glm::vec4(2.f);
	}

	// Other scene specific settings
	scene.initialize();
}


void App::reshape(int width, int height)
{
	scene.camera.setPerspective(width, height);
	display();
}


void App::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	scene.draw();

	glutSwapBuffers();

	//scene.camera.print();
}


void App::keyboard(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
}


void App::specialInput(int key, int x, int y)
{
	Camera* c = &scene.camera;
	float speed = 0.1f;

	switch (key) {
	case GLUT_KEY_UP:
		c->position.z += speed;
		break;
	case GLUT_KEY_DOWN:
		c->position.z -= speed;
		break;
	case GLUT_KEY_LEFT:
		c->position.x -= speed;
		break;
	case GLUT_KEY_RIGHT:
		c->position.x += speed;
		break;
	default:
		return;
	}

	glutPostRedisplay();
}


void App::mouseButton(int button, int state, int x, int y)
{
	if (state != GLUT_DOWN)
		return;
	if (button == GLUT_LEFT_BUTTON) {
		// Left mouse button pressed
		mousePos.x = x;
		mousePos.y = y;
	}
	// Wheel reports as button 3 (scroll up) and button 4 (scroll down)
	// Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
	if (button == 3) {
		// Scroll up
		Camera* c = &scene.camera;
		const glm::vec3& dir = c->lookAt - c->position;
		c->position = c->position + dir * 0.02f;
		glutPostRedisplay();
	}
	else if (button == 4) {
		// Scroll down
		Camera* c = &scene.camera;
		const glm::vec3& dir = c->lookAt - c->position;
		c->position = c->position - dir * 0.02f;
		glutPostRedisplay();
	}
}


void App::mouseMove(int x, int y)
{
	Camera* c = &scene.camera;
	float speed = 0.1f;

	c->rotation.x.w = (mousePos.y - y) * speed;
	c->rotation.y.w = (mousePos.x - x) * speed;

	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("DGI14 Project - Rainy Window");

	try {
		App();
	}
	catch (const std::exception& e) {
		fprintf(stderr, "error: %s\n", e.what());
		return 1;
	}
	return 0;
}

