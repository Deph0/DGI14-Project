#ifndef RAIN_DROPS_H
#define RAIN_DROPS_H

#include "scene.h"


class RainDrops : public Drawable {
public:
	RainDrops();

	void initialize();
	void draw() const;

private:
	Scene scene;
};

#endif // RAIN_DROPS_H

