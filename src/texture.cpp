#include "texture.h"
#include <SOIL.h>


void Texture::load(const std::string& fname) const //throw (TextureException)
{
	int width, height;
	unsigned char* image =
		SOIL_load_image(fname.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	if (image == NULL) {
		throw TextureException("failed to load texture image " + fname + \
			", " + SOIL_last_result());
	}

	select();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image);

	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

