#include "camera.h"
#include "opengl.h"
#include "sound.h"
#include "exception.h"


void Camera::initialize()
{
}


void Camera::setPerspective(int width, int height) const
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, width / (float)height, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
}


void Camera::setPosition(const glm::vec3& v)
{
	Object::setPosition(v);
	alListener3f(AL_POSITION, v.x, v.y, v.z);
}


void Camera::setLookAt(const glm::vec3& v)
{
	float orientation[6] = {
		v.x, v.y, v.z, upAxis.x, upAxis.y, upAxis.z
	};
	alListenerfv(AL_ORIENTATION, orientation);
	lookAt = v;
}


void Camera::setUpAxis(const glm::vec3& v)
{
	float orientation[6] = {
		lookAt.x, lookAt.y, lookAt.z, v.x, v.y, v.z
	};
	alListenerfv(AL_ORIENTATION, orientation);
	upAxis = v;
}


void Camera::draw() const
{
	const glm::vec3& pos = getPosition();

	gluLookAt(pos.x, pos.y, pos.z,
			  lookAt.x, lookAt.y, lookAt.z,
			  upAxis.x, upAxis.y, upAxis.z);
	//translate();
	Object::rotate();
	Object::scale();
}


void Camera::print() const
{
	Object::print();
	printf("  lookAt (%f, %f, %f)\n",
		lookAt.x, lookAt.y, lookAt.z);
	printf("  up axis (%f, %f, %f)\n",
		upAxis.x, upAxis.y, upAxis.z);
	printf("  fov: %f\n", fov);
	printf("  aspect: %f\n", aspect);
	printf("  z-near: %f\n", zNear);
	printf("  z-far: %f\n", zFar);
}

