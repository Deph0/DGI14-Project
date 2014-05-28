#include "object.h"
#include "opengl.h"
#include "glm/gtx/transform.hpp"


Object::Object()
: position(0.f)
, rotation(0.f)
, scaling(1.f)
{
}


Object::~Object()
{
	Sound::List::iterator i = sounds.begin();
	for (; i != sounds.end(); ++i) {
		delete *i;
	}
}


void Object::setPosition(const glm::vec3& v)
{
	position = v;
	// Move attached sounds
	Sound::List::iterator i = sounds.begin();
	for (; i != sounds.end(); ++i) {
		(*i)->setPosition(position);
	}
}


void Object::print() const
{
	printf("object [%s]\n", name.c_str());
	printf("  position (%f, %f, %f)\n",
		position.x, position.y, position.z);
	printf("  scale (%f, %f, %f)\n",
		scaling.x, scaling.y, scaling.z);
	printf("  rotation (%f, %f, %f)\n",
		rotation.x, rotation.y, rotation.z);
}


void Object::translate(const glm::vec3& v)
{
	setPosition(position + v);
}


void Object::rotate(const glm::vec3& v)
{
	setRotation(rotation + v);
}


void Object::translate() const
{
	glTranslatef(position.x, position.y, position.z);
}


void Object::rotate() const
{
	if (rotation.x != 0.f)
		glRotatef(rotation.x, 1.f, 0.f, 0.f);
	if (rotation.y != 0.f)
		glRotatef(rotation.y, 0.f, 1.f, 0.f);
	if (rotation.z != 0.f)
		glRotatef(rotation.z, 0.f, 0.f, 1.f);
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
		   glm::rotate(rotation.x, 1.f, 0.f, 0.f) * \
		   glm::rotate(rotation.y, 0.f, 1.f, 0.f) * \
		   glm::rotate(rotation.z, 0.f, 0.f, 1.f) * \
		   glm::scale(scaling);
}


Sound* Object::addSound()
{
	Sound* s = new Sound();
	sounds.push_back(s);
	s->setPosition(position);
	return s;
}

