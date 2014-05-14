#ifndef RAINY_DAY_H
#define RAINY_DAY_H

#include "glut_listener.h"
#include "singleton.h"
#include "rain_scene.h"
#include "speaker.h"


class RainyDay
	: public Singleton<RainyDay>
	, public Speaker<GlutListener>
{
public:
	RainyDay();
	virtual ~RainyDay() throw() { }

	void initialize();

private:
	static void reshape(int width, int height);
	static void display();
	static void keyDown(unsigned char key, int x, int y);
	static void specialKeyDown(int key, int x, int y);
	static void mouseButton(int button, int state, int x, int y);
	static void mouseMove(int x, int y);

	RainScene scene;
};

#endif // RAINY_DAY_H

