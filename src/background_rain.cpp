#include "background_rain.h"
#include "opengl.h"
#include "util.h"
#include "settings.h"


BackgroundRain::~BackgroundRain()
{
	RainDrop::Iter i = drops.begin();
	for (; i != drops.end(); ++i) {
		delete *i;
	}
}


void BackgroundRain::initialize()
{
}


void BackgroundRain::draw() const
{
	glPushAttrib(GL_LIGHTING);
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	RainDrop::cIter i = drops.begin();
	for (; i != drops.end(); ++i) {
		(*i)->draw();
	}
	glEnd();

	glPopAttrib();
}


bool BackgroundRain::animate()
{
	bool redraw = false;

	RainDrop::Iter i = drops.begin();
	for (; i != drops.end(); ++i) {
		redraw |= (*i)->update();
	}

	if (drops.size() < MAX_NR_DROPS_BGND) {
		int heaviness = std::min(HEAVINESS_OF_THE_RAIN,
			int(MAX_NR_DROPS_BGND - drops.size())) + 1;
		int nr = rand() % heaviness;

		createDrops(nr);
		if (nr > 0)
			redraw = true;
	}

	return redraw;
}


void BackgroundRain::createDrops(size_t cnt)
{
	for (; cnt > 0; cnt--) {
		drops.push_back(new RainDrop());
	}
}


BackgroundRain::RainDrop::RainDrop()
{
	start = glm::vec3(
		util::rand_range(-5.f, 5.f, 1000),
		util::rand_range(0.f, 4.f, 1000),
		7.f);
	speed = util::rand_range(0.15f, 0.45f, 10);
	alpha = 0.1f;
	position = start;
}


bool BackgroundRain::RainDrop::update()
{
	position.z -= speed;
	if (position.z < -1.f)
		position = start;
	return true;
}


void BackgroundRain::RainDrop::draw() const
{
	glColor4f(1.f, 1.f, 1.f, alpha);
	glVertex3fv(&position.x);
	glVertex3f(position.x, position.y, position.z - speed / 6.f);
}

