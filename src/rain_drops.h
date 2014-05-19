#ifndef RAIN_DROPS_H
#define RAIN_DROPS_H

#include "scene.h"
#include "shader_program.h"


class RainDrops : public Drawable {
public:
	RainDrops();

	void initialize();
	void draw() const;

	void animate();

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

		void newDirection();
		void newSpeed();

		Geometry* mesh;
		std::list<glm::vec3> path;
		bool showPath;
		glm::vec3 position;
		glm::vec3 direction;
		float distance;
		float speed;
		// If speed is zero begin to move again
		// after number of frames passed
		size_t moveAfterFrames;
	};

/*	class CollisionMap {
	public:
		void unset(const glm::vec3& pos);
		void set(const glm::vec3& pos);

		CollisionMap(const Rect* win, size_t accuracy);
		~CollisionMap();

	private:
		bool* map;
		const Rect* plane;
		float size;
	};*/

	Scene scene;
	Particle::List drops;
	ShaderProgram shaders;
//	CollisionMap* collisionMap;
};

#endif // RAIN_DROPS_H

