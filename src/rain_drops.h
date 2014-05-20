#ifndef RAIN_DROPS_H
#define RAIN_DROPS_H

#include "scene.h"
#include "shader_program.h"


class RainDrops : public Drawable {
public:
	void initialize();
	void draw() const;

	void animate();

	void setDrawingPlane(const Geometry* g) {
		g->getMinMax(&plane.min, &plane.max, true);
	}

	RainDrops();
	~RainDrops();

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

		struct {
			std::list<glm::vec3> positions;
			// Alpha channel of path color
			float alpha;
			// Line width
			float width;
		} path;

		Geometry* mesh;
		// Current position, calculates with help
		// of direction, distance and last start
		// position
		glm::vec3 position;
		glm::vec3 direction;
		// Distance form the last start position
		float distance;
		// Current speed of the drops
		float speed;
		// If speed is zero begin to move again
		// after number of frames passed
		size_t moveAfterFrames;
		// Randomized scaling of the drop
		glm::vec3 scaling;
		// In this mode particle is not drawn
		// but if it has a path it will be fading
		// out before it is removed
		bool fadingMode;
	};

	// Not the best implementation
	class CollisionMap {
	public:
		CollisionMap(float resolution)
		: factor(resolution)
		{ }

		void set(const glm::vec3& pos, Particle* p);
		Particle* get(const glm::vec3& pos);

	private:
		typedef std::map<int, Particle*> SubMap;
		typedef std::map<int, SubMap> Map;

		Map map;
		float factor;
	};

	Scene scene;
	Particle::List drops;
	ShaderProgram shaders;
	CollisionMap collision;
};

#endif // RAIN_DROPS_H

