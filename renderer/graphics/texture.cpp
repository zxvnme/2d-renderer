#define STB_IMAGE_IMPLEMENTATION

#include "texture.hpp"

#include "../thirdparty/glad/glad.hh"
#include "../thirdparty/stb/stb_image.h"

Texture::Texture(const int width, const int height, uint8_t* data)
	: width_(width)
	, height_(height)
	, channel_count_(4)
{
	glGenTextures(1, &this->texture_id_);
	glBindTexture(GL_TEXTURE_2D, this->texture_id_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width_, this->height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(const std::string_view filename)
{
	glGenTextures(1, &this->texture_id_);
	glBindTexture(GL_TEXTURE_2D, this->texture_id_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	const auto data = stbi_load(filename.data(), &this->width_, &this->height_, &this->channel_count_, 1 << 2);

	if (!data)
		std::exit(1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width_, this->height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->texture_id_);
}


unsigned int Texture::get_id() const
{
	return this->texture_id_;
}
