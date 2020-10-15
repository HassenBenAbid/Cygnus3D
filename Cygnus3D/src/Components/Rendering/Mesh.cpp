#include "Mesh.h"

namespace Cygnus3D {

	Mesh::Mesh(std::vector<glm::vec3> &positions, std::vector<unsigned int> &indices):
		m_positions(positions), m_indices(indices)
	{

	}

	Mesh::Mesh(std::vector<glm::vec3> &positions, std::vector<glm::vec2> &texCoords, std::vector<unsigned int> &indices):
		m_positions(positions), m_texCoords(texCoords), m_indices(indices)
	{

	}

	Mesh::Mesh(std::vector<glm::vec3> &positions, std::vector<glm::vec3> &normals, std::vector<unsigned int> &indices):
		m_positions(positions), m_normals(normals), m_indices(indices)
	{

	}

	Mesh::Mesh(std::vector<glm::vec3> &positions, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &normals, std::vector<unsigned int> &indices):
		m_positions(positions), m_texCoords(texCoords), m_normals(normals), m_indices(indices)
	{

	}

	void Mesh::setNormals(std::vector<glm::vec3> &normals) {
		m_normals = normals;
	}

	void Mesh::setTexCoords(std::vector<glm::vec2> &texCoords) {
		m_texCoords = texCoords;
	}

	Mesh::Mesh(const Mesh *mesh) {

		m_positions = mesh->m_positions;
		m_indices = mesh->m_indices;
		m_normals = mesh->m_normals;
		m_texCoords = mesh->m_texCoords;

		m_max = mesh->m_max;
		m_min = mesh->m_min;

		m_topology = mesh->m_topology;
		m_faceCulling = mesh->m_faceCulling;

		generate();

	}

	void Mesh::generate() {

		m_hasBones = false;

		m_min = m_positions[0];
		m_max = m_positions[0];

		for (int i = 1; i < m_positions.size(); i++) {

			if (m_positions[i].x < m_min.x) {
				m_min.x = m_positions[i].x;
			}

			if (m_positions[i].y < m_min.y) {
				m_min.y = m_positions[i].y;
			}

			if (m_positions[i].z < m_min.z) {
				m_min.z = m_positions[i].z;
			}

			if (m_positions[i].x > m_max.x) {
				m_max.x = m_positions[i].x;
			}

			if (m_positions[i].y > m_max.y) {
				m_max.y = m_positions[i].y;
			}

			if (m_positions[i].z > m_max.z) {
				m_max.z = m_positions[i].z;
			}
		}

		m_center = (m_max + m_min)/2.0f;

		if (!vao) {
			glGenVertexArrays(1, &vao);
			for (int i = 0; i < BUFFERS_NUMBER; i++) {
				glGenBuffers(1, &m_buffers[i]);
			}
			glGenBuffers(1, &vio);
		}

		glBindVertexArray(vao);

		if (m_indices.size() > 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vio);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_buffers[POSITION_LOCATION]);
		glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::vec3), &m_positions[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(POSITION_LOCATION);
		glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

		if (m_texCoords.size() > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[TEXCOORD_LOCATION]);
			glBufferData(GL_ARRAY_BUFFER, m_texCoords.size() * sizeof(glm::vec2), &m_texCoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(TEXCOORD_LOCATION);
			glVertexAttribPointer(TEXCOORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (m_normals.size() > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[NORMAL_LOCATION]);
			glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(NORMAL_LOCATION);
			glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (m_bonesID.size() > 0) {
			m_hasBones = true;
			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BONES_ID_LOCATION]);
			glBufferData(GL_ARRAY_BUFFER, m_bonesID.size() * sizeof(glm::ivec3), &m_bonesID[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(BONES_ID_LOCATION);
			glVertexAttribIPointer(BONES_ID_LOCATION, 3, GL_INT, 0, 0);
		}

		if (m_bonesWeight.size() > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[BONES_WEIGHT_LOCATION]);
			glBufferData(GL_ARRAY_BUFFER, m_bonesWeight.size() * sizeof(glm::vec3), &m_bonesWeight[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(BONES_WEIGHT_LOCATION);
			glVertexAttribPointer(BONES_WEIGHT_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		glBindVertexArray(0);

	}
}