#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"
#include "singleton.h"

class Sound;


class Camera
	: public Object
	, public Singleton<Camera>
{
public:
	void print() const;

	void initialize();
	void draw() const;

	void setPerspective(int width, int height) const;

	void setLookAt(const glm::vec3& v);
	void setUpAxis(const glm::vec3& v);
	// Override following methods to be able to move
	// the sound listener
	void setPosition(const glm::vec3& v);

	const glm::vec3& getLookAt() const {
		return lookAt;
	}

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

private:
	glm::vec3 lookAt;
	glm::vec3 upAxis;
};

#endif // CAMERA_H

