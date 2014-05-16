#ifndef RAIN_DROPS_H
#define RAIN_DROPS_H

#include "scene.h"
#include "shader_program.h"


class RainDrops : public Drawable {
public:
	RainDrops();

	void initialize();
	void draw() const;

	void setDrawingPlane(const glm::vec3& min, const glm::vec3& max) {
		planeMin = min;
		planeMax = max;
	}

private:
	Scene scene;
	glm::vec3 planeMin;
	glm::vec3 planeMax;
	ShaderProgram glass;
};

#endif // RAIN_DROPS_H

