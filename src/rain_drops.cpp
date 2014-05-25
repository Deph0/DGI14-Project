#include "rain_drops.h"
#include "obj_model.h"
#include "util.h"
#include "settings.h"
#include <algorithm>


RainDrops::RainDrops()
: collision(NULL)
{
}


RainDrops::~RainDrops()
{
	delete collision;
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

	collision = new CollisionTest(&plane, sqrt(MAX_NR_DROPS_ON_WINDOW));

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
	direction.x = util::rand_range(-0.2f, 0.2f, 100.f);
	direction.y = 0.f;
	// z spreading in range -0.6 - -0.1
	direction.z = util::rand_range(-0.6f, -0.1f, 10.f);
	distance = 0.f;
	newSpeed();
	path.positions.push_back(position);
}


void RainDrops::Particle::newSpeed()
{
	// Don't move too small ones
	if (scaling.x >= DROP_MOVE_MIN_SIZE && rand() % 101 > 80)
		speed = util::rand_range(0.005f, 0.02f, 1000.f);
	else {
		speed = 0.f;
		moveAfterFrames = rand() % 100 + 100;
	}
}


RainDrops::CollisionTest::CollisionTest(const RainDrops::Rect* rc, size_t size)
: plane(rc)
, resolution(size)
{
	factor = (plane->max - plane->min) / (float)resolution;
	cellMap.resize(size * size);
}


RainDrops::Particle* RainDrops::CollisionTest::check(RainDrops::Particle* p)
{
	Particle* ret = NULL;
	// Unmap previous position
	remove(p);

	// Get the index of the cell
	const glm::vec3& pos = p->position;
	// Is out of window? Don't care about this particle
	if (pos.x < plane->min.x || pos.z < plane->min.z ||
		pos.x > plane->max.x || pos.z > plane->max.z)
		return NULL;

	// Found the cell
	const glm::vec3& t = (pos - plane->min) / factor;
	if (t.z >= resolution || t.x >= resolution)
		return NULL;
	int idx = int(t.z) * resolution + int(t.x);

	// Map pointer to index to find it quickly when we need to remove it
	ptrIdxMap[p] = idx;
	// Find closest particle. This can be improved by check rectangle area
	// overlap or something. For now we stick to constant distance.
	Particle::List& lst = cellMap.at(idx);
	Particle::Iter i = lst.begin();
	float min = COLLISION_DISTANCE;
	for (; i != lst.end(); ++i) {
		float len = glm::length(p->position - (*i)->position);
		if (len < min) {
			min = len;
			ret = *i;
		}
	}
	// Add to this cell
	lst.push_back(p);
	// Return closest particle (can be NULL if nothing found)
	return ret;
}


void RainDrops::CollisionTest::remove(RainDrops::Particle* p)
{
	PtrIdxMap::iterator ptrI = ptrIdxMap.find(p);
	if (ptrI == ptrIdxMap.end())
		return;
	// Remove from the cell map
	Particle::List& lst = cellMap.at(ptrI->second);
	Particle::Iter pi = std::find(lst.begin(), lst.end(), p);
	if (pi != lst.end())
		lst.erase(pi);
	// Remove from the index map
	ptrIdxMap.erase(ptrI);
}


void RainDrops::createDrops(size_t cnt)
{
	size_t nrDrops = scene.geometries.size();
	Particle* drop;

	for (; cnt > 0; cnt--) {
		drop = new Particle();
		// Initialize
		drop->mesh = scene.geometries.at(rand() % nrDrops);
		drop->position = plane.randomize(DROPS_SPREADING_FACTOR);
		drop->newDirection();
		drop->path.alpha = util::rand_range(
			DROPS_PATH_ALPHA_MIN, DROPS_PATH_ALPHA_MAX, 100.f);
		drop->scaling = glm::vec3(
			util::rand_range(DROP_MIN_SIZE, DROP_MAX_SIZE, 1000.f));
		drop->fadingMode = false;
		drop->path.width = drop->scaling.x * 2.f;
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
		glColor4f(0.f, 0.f, 0.f, (*i)->path.alpha);
		float w = (*i)->path.width / 2.f;
		glBegin(GL_TRIANGLE_STRIP);
		pi = (*i)->path.positions.begin();
		glVertex3fv(&pi->x);
		while (++pi != (*i)->path.positions.end()) {
			glVertex3f(pi->x - w, pi->y, pi->z);
			glVertex3f(pi->x + w, pi->y, pi->z);
		}
		glVertex3f((*i)->position.x - w, (*i)->position.y, (*i)->position.z);
		glVertex3f((*i)->position.x + w, (*i)->position.y, (*i)->position.z);
		glEnd();
	}
	glPopAttrib();

	// Draw raindrops
	shaders.begin();
	//scene.geometries.at(rand() % scene.geometries.size())->draw();
	for (i = drops.begin(); i != drops.end(); ++i) {
		if ((*i)->fadingMode)
			continue;
		(*i)->mesh->position = (*i)->position;
		(*i)->mesh->scaling = (*i)->scaling;
		(*i)->mesh->draw();
	}
	shaders.end();
}


bool RainDrops::animate()
{
	bool draw = false;

	if (drops.size() < MAX_NR_DROPS_ON_WINDOW) {
		int heaviness = std::min(
			HEAVINESS_OF_THE_RAIN, int(MAX_NR_DROPS_ON_WINDOW - drops.size())) + 1;
		int nr = rand() % heaviness;
		createDrops(nr);
		if (nr > 0)
			draw = true;
	}

	Particle::Iter i = drops.begin();
	Particle* cur;

	while (i != drops.end()) {
		cur = *i;
		if (cur->fadingMode) {
			// The drop is in deleting state, fade out the path
			// before the drop with the path is removed
			cur->path.alpha -= DROPS_PATH_FADE_OUT;
			if (cur->path.alpha > 0.f)
				++i;
			else {
				// Don't forget to unmap from collision map
				collision->remove(cur);
				delete cur;
				i = drops.erase(i);
			}
			draw = true;
			continue;
		}
		if (cur->distance >= 1.f) {
			// We have arrived to end of direction path,
			// choose a new direction
			cur->newDirection();
		}
		if (cur->speed == 0.f) {
			// The drop is not moving, wait for a few frames
			// before choosing a new speed
			cur->moveAfterFrames--;
			if (cur->moveAfterFrames == 0) {
				cur->newSpeed();
			}
		}
		else { // The drop is moving
			if (cur->position.z < plane.min.z) {
				// The drop is below the window and not visible
				cur->fadingMode = true;
			}
			else { // The drop is on window and visible
				// Move in it's current direction
				cur->distance += cur->speed;
				cur->position =
					cur->path.positions.back() + \
					(cur->direction * cur->distance);
				draw = true;
			}
		}
		// Check for collision
		Particle* other = collision->check(cur);
		if (other) {
			// Join two drops (increase size)
			cur->scaling += other->scaling;
			if (cur->scaling.x > DROP_MAX_SIZE)
				cur->scaling = glm::vec3(DROP_MAX_SIZE);
			// Remove other
			other->fadingMode = true;
			draw = true;
		}
		++i;
	}

	return draw;
}

