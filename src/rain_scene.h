#ifndef RAIN_SCENE_H
#define RAIN_SCENE_H

#include "scene.h"
#include "rain_drops.h"
#include "glut_listener.h"


class RainScene : public GlutListener {
public:
	RainScene();

private:
	void on(GlutListener::Initialize);
	void on(GlutListener::Reshape, int width, int height);
	void on(GlutListener::Display);
	void on(GlutListener::SpecialKeyDown, unsigned char key, int x, int y);
	void on(GlutListener::MouseButton, int button, int state, int x, int y);
	void on(GlutListener::MouseMove, int x, int y);

	Scene scene;
	RainDrops raindrops;
	glm::ivec2 mousePos;
};

#endif // RAIN_SCENE_H

