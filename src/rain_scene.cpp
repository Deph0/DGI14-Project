#include "rain_scene.h"
#include "dae_model.h"
#include "point_light.h"
#include "util.h"
#include "settings.h"
#include "camera.h"


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
	Camera* camera = Camera::getInstance();
	DaeModel loader;

	loader.load(util::resource_path("starter_file.dae"), &scene, camera);

	// LookAt not present in dae file
	camera->setLookAt(glm::vec3(0.f, 0.f, 3.f));
	// Blender sets very strange rotation by default
	camera->setRotation(glm::vec3(0.f));
	// Setting the best view position found with
	// scene.camera.print() in onSpecialKeyDown function
	//c->setPosition(glm::vec3(-0.587549, -9.783041, 3.032476));
	camera->setPosition(glm::vec3(0, -9.204037, 3.032476));

	// Fix too darkness
	PointLight* lamp = (PointLight*)scene.getByName("InsideLamp");
	if (lamp) {
		lamp->color = lamp->color * 7.f;
	}
	Geometry* plane = (Geometry*)scene.getByName("BkgndPlane");
	if (plane) {
		plane->material->diffuse->color = glm::vec4(2.f);
	}

	// Initialize the main scene
	camera->initialize();
	scene.initialize();
	// Pick out the window glass to be drawn last because
	// of it's transparency
	glass = (Geometry*)scene.getByName("Glass", true);
	// Set plain where drops will appear
	raindrops.setDrawingPlane(glass);
	// Initialize the raindrops
	raindrops.initialize();

	// Background rain
	bgndRain.initialize();

	// Add sound of the rain
	Sound* rain = glass->addSound();
	rain->load(util::resource_path("rain_1.wav"));
	rain->enableLoop();
	rain->play();
	// Load thunders to be played at random time
	const char* thunders[] = {
		"thunderfade_1.wav", "loudthunderfade_1.wav", NULL
	};
	for (int i = 0; thunders[i]; i++) {
		Sound* t = glass->addSound();
		t->load(util::resource_path(thunders[i]));
		thunderSounds.push_back(t);
	}
}


void RainScene::on(GlutListener::Display)
{
	scene.draw();
	bgndRain.draw();
	raindrops.draw();
	glass->draw();
}


void RainScene::on(GlutListener::Idle, int deltaTime)
{
	bool redraw = false;
	float fps = 1000.f / deltaTime;
	time_t curTime = time(NULL);
	static time_t nextThunderTime = curTime + 5 + (rand() % 10);

	glutSetWindowTitle(util::format("%s - FPS: %4.2f - Next thunder in %u seconds",
		WINDOW_TITLE, fps, nextThunderTime - curTime).c_str());

	redraw |= raindrops.animate();
	redraw |= bgndRain.animate();

	if (redraw)
		glutPostRedisplay();

	if (nextThunderTime <= curTime) {
		int i = rand() % thunderSounds.size();
		thunderSounds[i]->play();
		// Randomize to next one
		nextThunderTime = rand() % 60 + 60 + curTime;
	}
}

void RainScene::on(GlutListener::KeyDown, unsigned char key, int x, int y)
{
	if (key == '+') {
		// Zoom in
		zoom(true);
	}
	else if (key == '-') {
		// Zoom out
		zoom(false);
	}
}

void RainScene::on(GlutListener::SpecialKeyDown, unsigned char key, int x, int y)
{
	Camera* c = Camera::getInstance();
	float speed = 0.1f;

	bool ctrl = glutGetModifiers() & GLUT_ACTIVE_CTRL;

	switch (key) {
	case GLUT_KEY_UP:
		c->translate(glm::vec3(0.f, 0.f, speed));
		break;
	case GLUT_KEY_DOWN:
		c->translate(glm::vec3(0.f, 0.f, -speed));
		break;
	case GLUT_KEY_LEFT:
		if (ctrl)
			c->rotate(glm::vec3(0.f, 0.f, 1.f));
		else
			c->translate(glm::vec3(-speed, 0.f, 0.f));
		break;
	case GLUT_KEY_RIGHT:
		if (ctrl)
			c->rotate(glm::vec3(0.f, 0.f, -1.f));
		else
			c->translate(glm::vec3(speed, 0.f, 0.f));
		break;
	default:
		return;
	}

	//c->print();
	glutPostRedisplay();
}


void RainScene::on(GlutListener::MouseButton, int button, int state, int x, int y)
{
	if (state != GLUT_DOWN)
		return;
	// Wheel reports as button 3 (scroll up) and button 4 (scroll down)
	// Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
	if (button == 3) {
		// Scroll up
		zoom(true);
	}
	else if (button == 4) {
		// Scroll down
		zoom(false);
	}
}


void RainScene::zoom(bool in)
{
	Camera* c = Camera::getInstance();
	const glm::vec3& dir = c->getLookAt() - c->getPosition();
	float speed = in ? 0.02f : -0.02f;

	c->translate(dir * speed);
	glutPostRedisplay();
}

