#include "directional_light.h"

DirectionalLight::DirectionalLight()
: Light()
{
}


void DirectionalLight::print() const
{
	Light::print();
	printf("  direction (%f, %f, %f)\n",
		direction.x, direction.y, direction.z);
}


void DirectionalLight::initialize()
{
	Light::initialize();
}


void DirectionalLight::draw() const
{
	// Indicates a directional light
	glm::vec4 dir(direction, 0.f);

	glLightfv(index, GL_POSITION, &dir.x);
	Light::draw();
}

