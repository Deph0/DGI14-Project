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
	glm::vec3 min, max;

	loader.load(util::resource_path("raindrops.obj"), &scene);
	scene.initialize();

	//const glm::vec3& center = (plane.min + plane.max) / 2.f;
	// Relocate
	Geometry::Iter i = scene.geometries.begin();
	for (; i != scene.geometries.end(); ++i) {
		(*i)->centerAtOrigin();
		//(*i)->position = center;
		(*i)->rotation.x.w = -90.f;
		(*i)->rotation.y.w = -90.f;
		(*i)->scaling = glm::vec3(0.06f);
	}

	createDrops(MAX_NR_DROPS);

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


void RainDrops::createDrops(size_t cnt)
{
	size_t nrDrops = scene.geometries.size();
	Particle drop;

	for (; cnt > 0; cnt--) {
		drop.mesh = scene.geometries.at(rand() % nrDrops);
		drop.position = plane.randomize(DROPS_SPREADING_FACTOR);
		drops.push_back(drop);
	}
}


void RainDrops::draw() const
{
	Particle::cIter i = drops.begin();

	shaders.begin();
	for (; i != drops.end(); ++i) {
		i->mesh->position = i->position;
		i->mesh->draw();
	}
	shaders.end();
}

