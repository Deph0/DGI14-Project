#include "rain_drops.h"
#include "obj_model.h"
#include "util.h"


RainDrops::RainDrops()
{
}


void RainDrops::initialize()
{
	ObjModel loader;

	loader.load(util::resource_path("raindrops.obj"), &scene);
	scene.initialize();

	const glm::vec3& center = (planeMin + planeMax) / 2.f;
	// Relocate
	Geometry::Iter i = scene.geometries.begin();
	for (; i != scene.geometries.end(); ++i) {
		(*i)->centerAtOrigin();
		(*i)->position = center;
		(*i)->rotation.x.w = -90.f;
		(*i)->rotation.y.w = -90.f;
		(*i)->scaling = glm::vec3(0.5f);
	}

	glass.createShader(GL_VERTEX_SHADER_ARB)->load(
		util::shader_path("raindrop.vert"));
	glass.createShader(GL_FRAGMENT_SHADER_ARB)->load(
		util::shader_path("raindrop.frag"));
	glass.initialize();
}


void RainDrops::draw() const
{
	Geometry* g = *(++scene.geometries.begin());
	glass.begin();
	g->draw();
	glass.end();
}

