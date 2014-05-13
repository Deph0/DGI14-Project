#include "point_light.h"

PointLight::PointLight()
: Light()
{
	attenuation.constant = 1.f;
	attenuation.linear = 0.f;
	attenuation.quadratic = 0.f;
}


void PointLight::print() const
{
	Light::print();
	printf("  attenuation (constant %f, linear %f, quadratic %f)\n",
		attenuation.constant, attenuation.linear, attenuation.quadratic);
}


void PointLight::initialize()
{
	Light::initialize();
}


void PointLight::draw() const
{
	// Indicates a point light
	glm::vec4 pos(position, 1.f);

	glLightfv(index, GL_POSITION, &pos.x);

	Light::draw();

	glLightf(index, GL_CONSTANT_ATTENUATION, attenuation.constant);
	glLightf(index, GL_LINEAR_ATTENUATION, attenuation.linear);
	glLightf(index, GL_QUADRATIC_ATTENUATION, attenuation.quadratic);
}

