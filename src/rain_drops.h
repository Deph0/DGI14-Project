#ifndef RAIN_DROPS_H
#define RAIN_DROPS_H

#include "scene.h"
#include "shader_program.h"


class RainDrops : public Drawable {
public:
	void initialize();
	void draw() const;

	// Update all drop's status.
	// Return true if drops need to be redrawn.
	bool animate();

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
		typedef std::list<Particle*> List;
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

	class CollisionTest {
	public:
		CollisionTest(const Rect* rc, size_t size);

		Particle* check(Particle* p);
		void remove(Particle* p);
//		void set(const glm::vec3& pos, Particle* p);
//		Particle* get(const glm::vec3& pos);

	private:
		typedef std::vector<Particle::List> CellMap;
		typedef std::map<Particle*, int> PtrIdxMap;

		const Rect* plane;
		size_t resolution;
		glm::vec3 factor;
		CellMap cellMap;
		PtrIdxMap ptrIdxMap;
	};

	Scene scene;
	Particle::List drops;
	ShaderProgram shaders;
	CollisionTest* collision;
};

#endif // RAIN_DROPS_H

