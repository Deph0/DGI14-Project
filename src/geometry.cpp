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


void Geometry::centerAtOrigin()
{
	// Find min and max of all vertices
	glm::vec3 min(std::numeric_limits<float>::max());
	glm::vec3 max(-std::numeric_limits<float>::max());
	glm::vec3 cur;
	size_t i = 0;

	for (; i < vertices.size(); i += 3) {
		cur.x = vertices.at(i);
		cur.y = vertices.at(i + 1);
		cur.z = vertices.at(i + 2);

		if (cur.x < min.x)
			min.x = cur.x;
		if (cur.y < min.y)
			min.y = cur.y;
		if (cur.z < min.z)
			min.z = cur.z;

		if (cur.x > max.x)
			max.x = cur.x;
		if (cur.y > max.y)
			max.y = cur.y;
		if (cur.z > max.z)
			max.z = cur.z;
	}

	// Translate all vertices
	cur = (max + min) / 2.f;
	for (i = 0; i < vertices.size(); i += 3) {
		vertices[i] -= cur.x;
		vertices[i + 1] -= cur.y;
		vertices[i + 2] -= cur.z;
	}
}

