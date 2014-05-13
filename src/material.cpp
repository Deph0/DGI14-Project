#include "material.h"
#include "texture.h"
#include <GL/gl.h>


Material::Material()
: emission(NULL)
, ambient(NULL)
, diffuse(NULL)
, specular(NULL)
, shininess(NULL)
, transparency(NULL)
{
}


Material::~Material()
{
	delete emission;
	delete ambient;
	delete diffuse;
	delete specular;
	delete shininess;
	delete transparency;
}


Material::Element::Element()
// Set default color to white without transparency
//: color(0.64, 0.64, 0.64, 1.f)
: color(1.f)
, texture(NULL)
{
}


Material::Element::~Element()
{
	delete texture;
}


void Material::initialize()
{
	Element* elem[] = { emission, ambient, diffuse, specular };
	int i = 0;

//	diffuse->color = diffuse->color * 2.56;
	for (; i < 4; i++) {
		if (elem[i] == NULL)
			continue;
		if (elem[i]->texture)
			elem[i]->texture->load();
		if (transparency) {
			elem[i]->color.w = *transparency;
		}
	}
//	ambient->color = specular->color;
}


bool Material::select() const
{
	Element* elem[] = { emission, ambient, diffuse, specular };
	GLenum type[] = { GL_EMISSION, GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR };
	GLenum sides = GL_FRONT_AND_BACK;
	bool texturePresent = false;
	int i = 0;

	for (; i < 4; i++) {
		if (elem[i] == NULL)
			continue;
		glMaterialfv(sides, type[i], &elem[i]->color.x);
		if (elem[i]->texture) {
//printf("texture color (%f, %f, %f, %f)\n", elem[i]->color.x, elem[i]->color.y, elem[i]->color.z, elem[i]->color.w);
			elem[i]->texture->select();
			texturePresent = true;
		}
	}
	if (specular && shininess) {
		glMaterialfv(sides, GL_SHININESS, shininess);
	}

	return texturePresent;
}


void Material::unselect() const
{
	Element* elem[] = { emission, ambient, diffuse, specular };
	int i = 0;

	for (; i < 4; i++) {
		if (elem[i] == NULL)
			continue;
		if (elem[i]->texture)
			elem[i]->texture->select(false);
	}
}

