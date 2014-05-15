#include "rain_drops.h"
#include "obj_model.h"


RainDrops::RainDrops()
{
}


void RainDrops::initialize()
{
	ObjModel loader;

	loader.load(RESOURCE_PATH + std::string("raindrops.obj"), &scene);
	scene.initialize();

	const glm::vec3& center = (planeMin + planeMax) / 2.f;
	// Relocate
	Geometry::Iter i = scene.geometries.begin();
	for (; i != scene.geometries.end(); ++i) {
		(*i)->centerAtOrigin();
		(*i)->position = center;
		(*i)->rotation.x.w = -90.f;
		(*i)->rotation.y.w = -90.f;
	}
}


void RainDrops::draw() const
{
	Geometry* g = *(++scene.geometries.begin());
	g->draw();
//	scene.draw();
}

