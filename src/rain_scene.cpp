#include "rain_scene.h"
#include "dae_model.h"
#include "point_light.h"
#include "util.h"
#include <glm/gtx/rotate_vector.hpp>


RainScene::RainScene()
: glass(NULL)
{
}


RainScene::~RainScene()
{
	delete glass;
}


void RainScene::on(GlutListener::Initialize)
{
	DaeModel loader;

	loader.load(util::resource_path("starter_file.dae"), &scene);

	Camera* c = scene.camera;
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
//	PointLight* sun = (PointLight*)scene.getByName("SunLamp");
//	if (sun)
//		sun->position.x += 4.f;
	Geometry* plane = (Geometry*)scene.getByName("BkgndPlane");
	if (plane) {
		plane->material->diffuse->color = glm::vec4(2.f);
	}

	// Initialize the main scene
	scene.initialize();
	// Pick out the window glass to be drawn last because
	// of it's transparency
	glass = (Geometry*)scene.getByName("Glass", true);
	// Set plain where drops will appear
	glm::vec3 min, max;
	glass->getMinMax(&min, &max);
	raindrops.setDrawingPlane(min, max);
	// Initialize the raindrops
	raindrops.initialize();
}


void RainScene::on(GlutListener::Reshape, int width, int height)
{
	scene.camera->setPerspective(width, height);
}


void RainScene::on(GlutListener::Display)
{
	scene.draw();
	raindrops.draw();
	glass->draw();
}


void RainScene::on(GlutListener::SpecialKeyDown, unsigned char key, int x, int y)
{
	Camera* c = scene.camera;
	float speed = 0.1f;

	bool ctrl = glutGetModifiers() & GLUT_ACTIVE_CTRL;

	switch (key) {
	case GLUT_KEY_UP:
		c->position.z += speed;
		break;
	case GLUT_KEY_DOWN:
		c->position.z -= speed;
		break;
	case GLUT_KEY_LEFT:
		if (ctrl)
			c->rotation.z.w += 1.f;
		else
			c->position.x -= speed;
		break;
	case GLUT_KEY_RIGHT:
		if (ctrl)
			c->rotation.z.w -= 1.f;
		else
			c->position.x += speed;
		break;
	default:
		return;
	}

	glutPostRedisplay();
}


void RainScene::on(GlutListener::MouseButton, int button, int state, int x, int y)
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
		Camera* c = scene.camera;
		const glm::vec3& dir = c->lookAt - c->position;
		c->position = c->position + dir * 0.02f;
		glutPostRedisplay();
	}
	else if (button == 4) {
		// Scroll down
		Camera* c = scene.camera;
		const glm::vec3& dir = c->lookAt - c->position;
		c->position = c->position - dir * 0.02f;
		glutPostRedisplay();
	}
}


void RainScene::on(GlutListener::MouseMove, int x, int y)
{
	Camera* c = scene.camera;
	float speed = 0.1f;

	c->rotation.x.w = (mousePos.y - y) * speed;
	c->rotation.y.w = (mousePos.x - x) * speed;

	glutPostRedisplay();
}

