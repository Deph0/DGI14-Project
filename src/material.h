#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include <string>
#include <glm/glm.hpp>

class Texture;


class Material {
public:
	typedef std::map<std::string, Material*> Map;
	typedef Map::const_iterator cIter;
	typedef Map::iterator Iter;

	class Element {
	public:
		Element();
		~Element();

		// OpenGL bind color to texture so
		// it must alsways be present
		glm::vec4 color;
		Texture* texture;
	};

	Material();
	~Material();

	// Load textures and set transparency
	void initialize();

	// Select current material.
	// Return true if texture is selected.
	bool select() const;
	// Disable textures
	void unselect() const;

	std::string name;

	Element* emission;
	Element* ambient;
	Element* diffuse;
	Element* specular;

	float* shininess;
	float* transparency;
};

#endif // MATERIAL_H

