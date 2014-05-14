#include "rain_scene.h"
#include "dae_model.h"
#include "point_light.h"


RainScene::RainScene()
{
}


void RainScene::on(GlutListener::Initialize)
{
	Camera* c = &scene.camera;
	DaeModel loader;

	loader.load(RESOURCE_PATH + std::string("starter_file.dae"), &scene);

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

	scene.initialize();
	raindrops.initialize();
}


void RainScene::on(GlutListener::Reshape, int width, int height)
{
	scene.camera.setPerspective(width, height);
}


void RainScene::on(GlutListener::Display)
{
	scene.draw();
	//raindrops.draw();
}


void RainScene::on(GlutListener::SpecialKeyDown, unsigned char key, int x, int y)
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


void RainScene::on(GlutListener::MouseMove, int x, int y)
{
	Camera* c = &scene.camera;
	float speed = 0.1f;

	c->rotation.x.w = (mousePos.y - y) * speed;
	c->rotation.y.w = (mousePos.x - x) * speed;

	glutPostRedisplay();
}

