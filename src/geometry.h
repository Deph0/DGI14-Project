#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <list>
#include <vector>
#include "object.h"

class Material;


class Geometry : public Object {
public:
	typedef std::list<Geometry*> List;
	typedef List::const_iterator cIter;
	typedef List::iterator Iter;

	Geometry()
	: material(NULL)
	{ }

	void draw() const;

	// Recalculate vertices to center object at (0, 0, 0)
	void centerAtOrigin();

	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texels;

	Material* material;
};

#endif // GEOMETRY_H

