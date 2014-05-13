#include "texture.h"
#include <SOIL/SOIL.h>
#include <stdio.h>


void Texture::load()
{
	handle = SOIL_load_OGL_texture(
		filename.c_str(), 3, 0, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y);
	if (handle == 0) {
		throw TextureException("failed to load texture image " + filename + \
			", " + SOIL_last_result());
	}
}


void Texture::select(bool en) const
{
	glBindTexture(GL_TEXTURE_2D, en ? handle : 0);
}

