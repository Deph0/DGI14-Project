#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include "object.h"

class Material;


class Geometry : public Object {
public:
	typedef std::vector<Geometry*> List;
	typedef List::const_iterator cIter;
	typedef List::iterator Iter;

	Geometry()
	: material(NULL)
	{ }

	void initialize();
	void draw() const;

	// Recalculate vertices to center object at (0, 0, 0)
	void centerAtOrigin();
	// Get min and max coordinates of the geometry
	void getMinMax(glm::vec3* min, glm::vec3* max, bool transformed = false) const;

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texels;

	Material* material;
};

#endif // GEOMETRY_H

