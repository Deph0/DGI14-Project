#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <string>
#include "drawable.h"
#include "sound.h"


class Object : public Drawable {
public:
	Object();
	~Object();

	void print() const;

	// Following altering the members
	void translate(const glm::vec3& v);
	void rotate(const glm::vec3& v);

	// Draw functions
	void translate() const;
	void rotate() const;
	void scale() const;
	// Do translate, rotate and scale
	void transform() const;

	glm::mat4 getTransformMatrix() const;

	// Attach a new sound
	Sound* addSound();

	// Seters
	virtual void setPosition(const glm::vec3& v);
	void setRotation(const glm::vec3& v) {
		rotation = v;
	}
	void setScaling(const glm::vec3& v) {
		scaling = v;
	}
	// Geters
	const glm::vec3& getPosition() const {
		return position;
	}

	std::string name;

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scaling;

	// Attached sounds
	Sound::List sounds;
};

#endif // OBJECT_H

