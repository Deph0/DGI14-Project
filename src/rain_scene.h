#ifndef RAIN_SCENE_H
#define RAIN_SCENE_H

#include "scene.h"
#include "rain_drops.h"
#include "background_rain.h"
#include "glut_listener.h"

class Sound;


class RainScene : public GlutListener {
public:
	RainScene();
	~RainScene();

private:
	void on(GlutListener::Initialize);
	void on(GlutListener::Display);
	void on(GlutListener::Idle, int deltaTime);
	void on(GlutListener::SpecialKeyDown, unsigned char key, int x, int y);
	void on(GlutListener::KeyDown, unsigned char key, int x, int y);
	void on(GlutListener::MouseButton, int button, int state, int x, int y);

	// Zoom in (true) or out (false)
	void zoom(bool in);

	Scene scene;
	RainDrops raindrops;
	BackgroundRain bgndRain;
	Geometry* glass;
	Sound::Vector thunderSounds;
};

#endif // RAIN_SCENE_H

