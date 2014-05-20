#include "rain_drops.h"
#include "obj_model.h"
#include "util.h"
#include "settings.h"


RainDrops::RainDrops()
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
		(*i)->scaling = glm::vec3(0.04f);
	}

//	createDrops(MAX_NR_DROPS);
	glLineWidth(3.f);

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
	// x spreading in range -0.4 - 0.4
	direction.x = util::in_range(-0.2f, 0.2f, 100.f);
	direction.y = 0.f;
	// z spreading in range -0.1 - -0.6
	direction.z = util::in_range(-0.6f, -0.1f, 10.f);
	distance = 0.f;
	newSpeed();
	path.push_back(position);
}


void RainDrops::Particle::newSpeed()
{
	// speed is in range 0.1 - 0.02
	if (rand() & 1)
		speed = (rand() % 81) / 1000.f + 0.02f;
	else {
		speed = 0.f;
		moveAfterFrames = rand() % 100 + 1;
	}
//	speed = 0.05f;
}


void RainDrops::createDrops(size_t cnt)
{
	size_t nrDrops = scene.geometries.size();
	Particle drop;

	for (; cnt > 0; cnt--) {
		drop.mesh = scene.geometries.at(rand() % nrDrops);
		drop.position = plane.randomize(DROPS_SPREADING_FACTOR);
		drop.newDirection();
		// Only specified percentage of drops should have path trace
		drop.showPath = (rand() % 100) >= (100 - DROPS_PATH_TRACE_PERCENT);
		drops.push_back(drop);
	}
}


void RainDrops::draw() const
{
	Particle::cIter i = drops.begin();
	std::list<glm::vec3>::const_iterator pi;

	// Draw pathes
	glDisable(GL_LIGHTING);
	glColor4f(0.f, 0.f, 0.f, 0.1f);
	for (; i != drops.end(); ++i) {
		if (!i->showPath)
			continue;
		glBegin(GL_LINE_STRIP);
		for (pi = i->path.begin(); pi != i->path.end(); ++pi) {
			glVertex3fv(&pi->x);
		}
		glVertex3fv(&i->position.x);
		glEnd();
	}
	glEnable(GL_LIGHTING);

	// Draw raindrops
	shaders.begin();
	//scene.geometries.at(rand() % scene.geometries.size())->draw();
	for (i = drops.begin(); i != drops.end(); ++i) {
		i->mesh->position = i->position;
		i->mesh->draw();
	}
	shaders.end();
}


void RainDrops::animate()
{
	if (drops.size() < MAX_NR_DROPS) {
		createDrops(1);
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
			i->distance += i->speed;
			i->position = i->path.back() + (i->direction * i->distance);

			if (i->position.z < plane.min.z) {
				i = drops.erase(i);
				continue;
			}
		}
		++i;
	}

	glutPostRedisplay();
}

