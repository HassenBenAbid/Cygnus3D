#include "Quad.h"

namespace Cygnus3D {

	Quad::Quad() {
		m_positions = {
			glm::vec3(-0.5f, -0.5f,  0.5f),
			glm::vec3(0.5f, -0.5f,  0.5f),
			glm::vec3(0.5f,  0.5f,  0.5f),
			glm::vec3(-0.5f,  0.5f,  0.5f)
		};

		m_texCoords = {
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f)
		};

		m_normals = {
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		};

		m_indices = {
			0,  1,  2,      
			0,  2,  3
		};

		m_faceCulling = false;
		generate();
	}

	Quad::Quad(float multiplier) {
		m_positions = {
			multiplier * glm::vec3(-0.5f, -0.5f,  0.5f),
			multiplier * glm::vec3(0.5f, -0.5f,  0.5f),
			multiplier * glm::vec3(0.5f,  0.5f,  0.5f),
			multiplier * glm::vec3(-0.5f,  0.5f,  0.5f)
		};

		m_texCoords = {
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f)
		};

		m_normals = {
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		};

		m_indices = {
			0,  1,  2,
			0,  2,  3
		};

		m_faceCulling = false;
		generate();
	}

}