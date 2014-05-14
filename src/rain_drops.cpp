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

	// Relocate
	Geometry::Iter i = scene.geometries.begin();
	for (; i != scene.geometries.end(); ++i) {
		(*i)->centerAtOrigin();
//		(*i)->position = glm::vec3(0.f, 2.f, 0.f);
		(*i)->rotation.x.w = -90.f;
		(*i)->rotation.y.w = -90.f;
	}
}


void RainDrops::draw() const
{
/*	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

	Geometry* g = *(++scene.geometries.begin());
	g->draw();

	glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);*/
//	scene.draw();
}

