#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "light.h"

class PointLight : public Light {
public:
	PointLight();

	void initialize();
	void draw() const;
	void print() const;

	Light::Type getType() const {
		return POINT_LIGHT;
	}

	struct {
		float constant;
		float linear;
		float quadratic;
	} attenuation;
};

#endif // POINT_LIGHT_H

