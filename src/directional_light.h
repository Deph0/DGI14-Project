#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "light.h"

class DirectionalLight : public Light {
public:
	DirectionalLight();

	void initialize();
	void draw() const;
	void print() const;

	Light::Type getType() const {
		return DIRECTIONAL_LIGHT;
	}

	glm::vec3 direction;
};

#endif // DIRECTIONAL_LIGHT_H

