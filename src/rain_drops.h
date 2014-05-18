#ifndef RAIN_DROPS_H
#define RAIN_DROPS_H

#include "scene.h"
#include "shader_program.h"


class RainDrops : public Drawable {
public:
	RainDrops();

	void initialize();
	void draw() const;

	void setDrawingPlane(const Geometry* g) {
		g->getMinMax(&plane.min, &plane.max, true);
	}

private:
	void createDrops(size_t cnt);

	struct Rect {
		glm::vec3 randomize(int accuracy) const;

		glm::vec3 min;
		glm::vec3 max;
	} plane;

	struct Particle {
		typedef std::list<Particle> List;
		typedef List::const_iterator cIter;
		typedef List::iterator Iter;

		Geometry* mesh;
		glm::vec3 position;
	};

	Scene scene;
	Particle::List drops;
	ShaderProgram shaders;
};

#endif // RAIN_DROPS_H

