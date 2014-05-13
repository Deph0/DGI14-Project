#include "geometry.h"
#include "material.h"
#include "opengl.h"


void Geometry::draw() const
{
	bool texture = false;

	glPushMatrix();
	Object::transform();

	if (material) {
		texture = material->select();
		if (texture) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, texels.data());
		}
	}

	glVertexPointer(3, GL_FLOAT, 0, vertices.data());
	glNormalPointer(GL_FLOAT, 0, normals.data());
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

	if (texture) {
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		material->unselect();
	}

	glPopMatrix();
}

