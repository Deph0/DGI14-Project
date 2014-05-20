#include "object.h"
#include "opengl.h"
#include "glm/gtx/transform.hpp"


Object::Object()
: position(0.f)
, scaling(1.f)
{
	resetRotation();
}


void Object::resetRotation()
{
	rotation.x = glm::vec4(1.f, 0.f, 0.f, 0.f);
	rotation.y = glm::vec4(0.f, 1.f, 0.f, 0.f);
	rotation.z = glm::vec4(0.f, 0.f, 1.f, 0.f);
}


void Object::print() const
{
	printf("object [%s]\n", name.c_str());
	printf("  position (%f, %f, %f)\n",
		position.x, position.y, position.z);
	printf("  scale (%f, %f, %f)\n",
		scaling.x, scaling.y, scaling.z);
	printf("  rotation\n");
	printf("	x-axis (%f, %f, %f, %f)\n",
		rotation.x.x, rotation.x.y, rotation.x.z, rotation.x.w);
	printf("	y-axis (%f, %f, %f, %f)\n",
		rotation.y.x, rotation.y.y, rotation.y.z, rotation.y.w);
	printf("	z-axis (%f, %f, %f, %f)\n",
		rotation.z.x, rotation.z.y, rotation.z.z, rotation.z.w);
}



void Object::translate() const
{
	glTranslatef(position.x, position.y, position.z);
}


void Object::rotate() const
{
	glRotatef(
		rotation.x.w, rotation.x.x, rotation.x.y, rotation.x.z);
	glRotatef(
		rotation.y.w, rotation.y.x, rotation.y.y, rotation.y.z);
	glRotatef(
		rotation.z.w, rotation.z.x, rotation.z.y, rotation.z.z);
}


void Object::scale() const
{
	glScalef(scaling.x, scaling.y, scaling.z);
}


void Object::transform() const
{
	translate();
	rotate();
	scale();
}


glm::mat4 Object::getTransformMatrix() const
{
	return glm::translate(position) * \
		   glm::rotate(rotation.x.w, rotation.x.x, rotation.x.y, rotation.x.z) * \
		   glm::rotate(rotation.y.w, rotation.y.x, rotation.y.y, rotation.y.z) * \
		   glm::rotate(rotation.z.w, rotation.z.x, rotation.z.y, rotation.z.z) * \
		   glm::scale(scaling);
}

