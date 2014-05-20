#include "rain_drops.h"
#include "obj_model.h"
#include "util.h"
#include "settings.h"


RainDrops::RainDrops()
: collision(10.f)
{
}


RainDrops::~RainDrops()
{
}


void RainDrops::initialize()
{
	ObjModel loader;

	loader.load(util::resource_path("raindrops.obj"), &scene);
	scene.initialize();

//	const glm::vec3& center = (plane.min + plane.max) / 2.f;
	// Relocate
	Geometry::Iter i = scene.geometries.begin();
	for (; i != scene.geometries.end(); ++i) {
		(*i)->centerAtOrigin();
//		(*i)->position = center;
//		(*i)->rotation.z.w = 180.f;
//		(*i)->scaling = glm::vec3(0.04f);
	}

//	createDrops(MAX_NR_DROPS);

	shaders.createShader(GL_VERTEX_SHADER_ARB)->load(
		util::shader_path("raindrop.vert"));
	shaders.createShader(GL_FRAGMENT_SHADER_ARB)->load(
		util::shader_path("raindrop.frag"));
	shaders.initialize();
}


glm::vec3 RainDrops::Rect::randomize(int accuracy) const
{
	const glm::vec3& delta = max - min;
	glm::vec3 res;
	float inverse = 1.f / accuracy;

	res.x = delta.x * ((rand() % (accuracy + 1)) * inverse);
	res.y = delta.y * ((rand() % (accuracy + 1)) * inverse);
	res.z = delta.z * ((rand() % (accuracy + 1)) * inverse);

	return res + min;
}


void RainDrops::Particle::newDirection()
{
	// x spreading in range -0.2 - 0.2
	direction.x = util::in_range(-0.2f, 0.2f, 100.f);
	direction.y = 0.f;
	// z spreading in range -0.6 - -0.1
	direction.z = util::in_range(-0.6f, -0.1f, 10.f);
	distance = 0.f;
	newSpeed();
	path.positions.push_back(position);
}


void RainDrops::Particle::newSpeed()
{
	// Don't move to small ones
	if (scaling.x >= DROP_MOVE_MIN_SIZE && rand() % 101 > 80)
		speed = util::in_range(0.005f, 0.02f, 1000.f);
	else {
		speed = 0.f;
		moveAfterFrames = rand() % 100 + 100;
	}
}


void RainDrops::CollisionMap::set(const glm::vec3& pos, RainDrops::Particle* p)
{
	int x = round(pos.x * factor);
	int y = round(pos.z * factor);

	map[y][x] = p;
}


RainDrops::Particle* RainDrops::CollisionMap::get(const glm::vec3& pos)
{
	int x = round(pos.x * factor);
	int y = round(pos.z * factor);

	Map::iterator mi = map.find(y);
	if (mi == map.end())
		return NULL;
	SubMap::iterator si = mi->second.find(x);
	if (si == mi->second.end())
		return NULL;
	return si->second;
}

//	factor = (plane->max - plane->min) / (float)resolution;
//	const glm::vec3& t = (pos - plane->min) / factor;

void RainDrops::createDrops(size_t cnt)
{
	size_t nrDrops = scene.geometries.size();

	for (; cnt > 0; cnt--) {
		Particle drop;
		drop.mesh = scene.geometries.at(rand() % nrDrops);
		drop.position = plane.randomize(DROPS_SPREADING_FACTOR);
		drop.newDirection();
		drop.path.alpha = util::in_range(
			DROPS_PATH_ALPHA_MIN, DROPS_PATH_ALPHA_MAX, 100.f);
		drop.scaling = glm::vec3(
			util::in_range(DROP_MIN_SIZE, DROP_MAX_SIZE, 1000.f));
		drop.fadingMode = false;
	#ifdef USING_LINES
		// The following constant factor should be depend on zooming
		// factor and window size
		drop.path.width = drop.scaling.x * 150.f;
	#else // not USING_LINES
		drop.path.width = drop.scaling.x * 2.f;
	#endif
		drops.push_back(drop);
	}
}


void RainDrops::draw() const
{
	Particle::cIter i = drops.begin();
	std::list<glm::vec3>::const_iterator pi;

	// Draw pathes
	glPushAttrib(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	for (; i != drops.end(); ++i) {
		glColor4f(0.f, 0.f, 0.f, i->path.alpha);
	#ifdef USING_LINES
		glLineWidth(i->path.width);
		glBegin(GL_LINE_STRIP);
		pi = i->path.positions.begin();
		for (; pi != i->path.positions.end(); ++pi) {
			glVertex3fv(&pi->x);
		}
		glVertex3fv(&i->position.x);
	#else // not USING_LINES
		float w = i->path.width / 2.f;
		glBegin(GL_TRIANGLE_STRIP);
		pi = i->path.positions.begin();
		glVertex3fv(&pi->x);
		while (++pi != i->path.positions.end()) {
			glVertex3f(pi->x - w, pi->y, pi->z);
			glVertex3f(pi->x + w, pi->y, pi->z);
		}
		glVertex3f(i->position.x - w, i->position.y, i->position.z);
		glVertex3f(i->position.x + w, i->position.y, i->position.z);
	#endif
		glEnd();
	}
	glPopAttrib();

	// Draw raindrops
	shaders.begin();
	//scene.geometries.at(rand() % scene.geometries.size())->draw();
	for (i = drops.begin(); i != drops.end(); ++i) {
		if (i->fadingMode)
			continue;
		i->mesh->position = i->position;
		i->mesh->scaling = i->scaling;
		i->mesh->draw();
	}
	shaders.end();
}


void RainDrops::animate()
{
	if (drops.size() < MAX_NR_DROPS) {
		int heaviness = std::min(
			HEAVINESS_OF_THE_RAIN, int(MAX_NR_DROPS - drops.size())) + 1;
		createDrops(rand() % heaviness);
	}

	Particle::Iter i = drops.begin();
	while (i != drops.end()) {
		if (i->distance >= 1.f) {
			i->newDirection();
		}
		if (i->speed == 0.f) {
			i->moveAfterFrames--;
			if (i->moveAfterFrames == 0) {
				i->newSpeed();
			}
		}
		else {
			if (i->fadingMode) {
				i->path.alpha -= DROPS_PATH_FADE_OUT;
				if (i->path.alpha > 0.f)
					++i;
				else {
					collision.set(i->position, NULL);
					i = drops.erase(i);
				}
				continue;
			}

			if (i->position.z < plane.min.z) {
				i->fadingMode = true;
			}
			else {
				collision.set(i->position, NULL);

				i->distance += i->speed;
				i->position = i->path.positions.back() + (i->direction * i->distance);

				Particle* p = collision.get(i->position);
				if (p) {
					p->scaling += i->scaling;
					if (p->scaling.x > DROP_MAX_SIZE)
						p->scaling = glm::vec3(DROP_MAX_SIZE);
					i->fadingMode = true;
				}
				else {
					collision.set(i->position, &(*i));
				}
			}
		}
		++i;
	}

	glutPostRedisplay();
}

