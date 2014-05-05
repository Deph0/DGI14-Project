#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <list>
#include <GL/gl.h>

class Texture;

struct Material {
	typedef std::list<Material*> List;
	typedef List::iterator Iter;

	Material(const std::string& n)
	: name(n)
	, diffuseTexture(NULL)
	, specularWeight(0.f)
//	, transparency(0.f)
	, opticalDensity(0.f)
	, illuminationModel(-1)
	{ }

	void setTransparency(float t) {
		ambient[3] = diffuse[3] = specular[3] = t;
	}

	void apply() const {
		glMaterialfv(GL_FRONT, GL_SPECULAR, ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT, specular);
		glMaterialf(GL_FRONT, GL_SHININESS, specularWeight);
	}

	// Name of the material
	std::string name;

	// The ambient color of the material is declared
	// using Ka. Color definitions are in RGB where
	// each channel's value is between 0 and 1
	float ambient[4];

	// Similarly, the diffuse color is declared using Kd
	float diffuse[4];
	Texture* diffuseTexture;

	// The specular color is declared using Ks, and
	// weighted using the specular coefficient Ns
	float specular[4];
	float specularWeight;

	// Materials can be transparent. This is referred
	// to as being dissolved. Unlike real transparency,
	// the result does not depend upon the thickness
	// of the object.
//	float transparency;

	// Specifies the optical density for the surface.
	// This is also known as index of refraction.
	float opticalDensity;

	// The illumination model to use in the material.
	//
	// 0   Color on and Ambient off
	// 1   Color on and Ambient on
	// 2   Highlight on
	// 3   Reflection on and Ray trace on
	// 4   Transparency: Glass on
	//     Reflection: Ray trace on
	// 5   Reflection: Fresnel on and Ray trace on
	// 6   Transparency: Refraction on
	//     Reflection: Fresnel off and Ray trace on
	// 7   Transparency: Refraction on
	//     Reflection: Fresnel on and Ray trace on
	// 8   Reflection on and Ray trace off
	// 9   Transparency: Glass on
	//     Reflection: Ray trace off
	// 10  Casts shadows onto invisible surfaces
	int illuminationModel;
};

#endif // MATERIAL_H

