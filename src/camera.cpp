#include "camera.h"
#include "opengl.h"

void Camera::setPerspective(int width, int height) const
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, width / (float)height, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
}


void Camera::draw() const
{
	gluLookAt(position.x, position.y, position.z,
			  lookAt.x, lookAt.y, lookAt.z,
			  upAxis.x, upAxis.y, upAxis.z);
	//translate();
	rotate();
	scale();
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

