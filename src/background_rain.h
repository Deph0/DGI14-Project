#ifndef BACKGROUND_RAIN_H
#define BACKGROUND_RAIN_H

#include "drawable.h"
#include <list>
#include <glm/glm.hpp>


class BackgroundRain : public Drawable {
public:
	void initialize();
	void draw() const;

	bool animate();

	~BackgroundRain();

private:
	void createDrops(size_t cnt);

	struct RainDrop {
		typedef std::list<RainDrop*> List;
		typedef List::const_iterator cIter;
		typedef List::iterator Iter;

		RainDrop();

		bool update();
		void draw() const;

		// Initial position
		glm::vec3 start;
		// Current position
		glm::vec3 position;
		// Constant fall speed
		float speed;
		// Transparency
		float alpha;
	};

	RainDrop::List drops;
};

#endif // BACKGROUND_RAIN_H

