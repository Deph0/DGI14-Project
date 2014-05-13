#ifndef APP_H
#define APP_H

#include "scene.h"


class App {
public:
	App();

private:
	void initialize();

	static void display();
	static void reshape(int width, int height);
	static void keyboard(unsigned char key, int x, int y);
	static void specialInput(int key, int x, int y);
	static void mouseMove(int x, int y);
	static void mouseButton(int button, int state, int x, int y);

	static Scene scene;
	static glm::ivec2 mousePos;
};

#endif // APP_H

