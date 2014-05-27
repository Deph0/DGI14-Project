#ifndef SCENE_H
#define SCENE_H

#include "light.h"
#include "geometry.h"
#include "material.h"
#include "texture.h"


class Scene : public Drawable {
public:
	void initialize();
	void draw() const;

	// Return NULL if not found
	Object* getByName(const std::string& name, bool erase = false);

	std::string name;

	Light::List lights;
	Geometry::List geometries;

	Material::Map materials;

	Scene();
	~Scene();
};

#endif // SCENE_H

