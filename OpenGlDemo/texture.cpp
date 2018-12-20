#include "texture.h"
#include "stb_image.h"

Texture::Texture(std::string filepath)
{
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &colorChannels, STBI_rgb);

	glGenTextures(1, &textureid);
	glBindTexture(GL_TEXTURE_2D, textureid);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);
}
