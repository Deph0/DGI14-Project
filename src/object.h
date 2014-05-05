#ifndef OBJECT_H
#define OBJECT_H

#include <list>
#include <vector>
#include <string>

class Material;

struct Object {
	typedef std::list<Object*> List;
	typedef List::const_iterator cIter;
	typedef List::iterator Iter;

	Object(const std::string& n)
	: name(n)
	, smoothShading(false)
	, material(NULL)
	{ }

	std::string name;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texels;
	bool smoothShading;
	Material* material;
};

#endif // OBJECT_H

