#include "light.h"
#include <GL/glut.h>

GLenum Light::lightIndex = GL_LIGHT0;


Light::Light()
: color(0, 0, 0, 1)
{
	index = lightIndex++;
}


void Light::print() const
{
	Object::print();
	printf("  color (%f, %f, %f, %f)\n",
		color.r, color.g, color.b, color.w);
}


void Light::initialize()
{
}


void Light::draw() const
{
	glLightfv(index, GL_AMBIENT, &color.r);
	glLightfv(index, GL_DIFFUSE, &color.r);
	glLightfv(index, GL_SPECULAR, &color.r);

	/*glPushMatrix();
		float col[4] = { 50, 50, 0, 1 };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, col);
		glTranslatef(position.x, position.y, position.z);
		glutSolidSphere(0.75, 20, 20);
	glPopMatrix();*/
}


void Light::enable(bool en) const
{
	if (en)
		glEnable(index);
	else
		glDisable(index);
}

