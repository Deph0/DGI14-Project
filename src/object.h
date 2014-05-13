#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <string>
#include "drawable.h"


class Object : public Drawable {
public:
	Object();

	void print() const;

	void translate() const;
	void rotate() const;
	void scale() const;
	// Do translate, rotate and scale
	void transform() const;
	// Set all rotations to zero angle
	void resetRotation();

	std::string name;

	glm::vec3 position;
	glm::vec3 scaling;

	struct {
		glm::vec4 x;
		glm::vec4 y;
		glm::vec4 z;
	} rotation;
};

#endif // OBJECT_H

