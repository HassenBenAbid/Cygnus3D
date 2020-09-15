#include "Texture.h"
#include <GLEW/glew.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <STB_IMAGE/stb_image.h>

namespace Cygnus3D {

	Texture::Texture(const char *filePath) {

		stbi_set_flip_vertically_on_load(false);

		m_data = stbi_load(filePath, &m_width, &m_height, &m_nbrChannels, 0);

		if (m_data) {

			glGenTextures(1, &m_textureID);
			glBindTexture(GL_TEXTURE_2D, m_textureID);

			GLenum format;
			GLint param;
			if (m_nbrChannels == 1) {
				format = GL_RED;
				param = GL_REPEAT;
			}
			else if (m_nbrChannels == 3) {
				format = GL_RGB;
				param = GL_REPEAT;
			}
			else if (m_nbrChannels == 4) {
				format = GL_RGBA;
				param = GL_CLAMP_TO_EDGE;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_data);


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(m_data);

			glBindTexture(GL_TEXTURE_2D, 0);

			std::cout << "IMAGE LOADED!" << std::endl;
		}
		else {
			std::cout << "CAN'T LOAD IMAGE!" << std::endl;
		}

	}

	Texture::~Texture() {
		glDeleteTextures(1, &m_textureID);
	}

	void Texture::bind() {
		glActiveTexture(GL_TEXTURE0 + m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}

	void Texture::unbind() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}