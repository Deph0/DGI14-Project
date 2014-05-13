#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"


class Camera : public Object {
public:
	void print() const;

	void draw() const;

	void setPerspective(int width, int height) const;

	glm::vec3 lookAt;
	glm::vec3 upAxis;

	//----------------------------------
	// Perspective projection
	//----------------------------------

	// Field ov view angle, in degrees, in
	// the y direction
	float fov;
	// The aspect ratio that determines the
	// field of view in the x direction
	float aspect;
	// The distance from the viewer to the
	// near clipping plane (always positive)
	float zNear;
	// The distance from the viewer to the
	// far clipping plane (always positive)
	float zFar;
};

#endif // CAMERA_H

