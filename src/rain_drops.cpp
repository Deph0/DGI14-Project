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
}


void RainDrops::draw() const
{
	scene.draw();
}

