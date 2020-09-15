#include "Cubemap.h"
#include <GLEW/glew.h>
#include <iostream>
#include <STB_IMAGE/stb_image.h>

namespace Cygnus3D {
	Cubemap::Cubemap(std::vector<const char*> facesPath) {

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

		for (unsigned int i = 0; i < facesPath.size(); i++) {
			m_data = stbi_load(facesPath[i], &m_width, &m_height, &m_nbrChannels, 0);
			if (m_data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_data);
			}
			else {
				std::cout << "CUBEMAP FAILED TO LOAD!" << std::endl;
			}

			stbi_image_free(m_data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void Cubemap::bind() {
		glActiveTexture(GL_TEXTURE0 + m_textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
	}

	void Cubemap::unbind() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}